import sys
import pandas as pd
import os
from Plot import lgraph

SHUNT_VALUE = 10

TIME_LABEL = 3
SUPPLY_PROBE_LABEL = 4
SHUNT_PROBE_LABEL = 10
WORKING_SIGNAL_PROBE_LABEL = 16
COMPRESSING_SIGNAL_PROBE_LABEL = 22

OUTPUT_TIME_LABEL = 'Time(S)'
OUTPUT_CURRENT_LABEL = 'Current(A)'
OUTPUT_VOLTAGE_LABEL = 'Voltage(V)'
OUTPUT_POWER_USAGE_LABEL = 'Power(W)'
OUTPUT_CONSUMED_ENERGY_LABEL = 'Energy(J)'
OUTPUT_POWER_DURING_COMPRESSION_LABEL = 'PowerDuringCompression(W)'
OUTPUT_CONSUMED_ENERGY_DURING_COMPRESSION = 'EnergyDuringCompression(J)'
OUTPUT_POWER_DURING_TRANSMISSION_LABEL = 'PowerDuringTransmission(W)'
OUTPUT_CONSUMED_ENERGY_DURING_TRANSMISSION = 'EnergyDuringTransmission(J)'


def main(argv):
    path = argv[0]
    directory = os.path.dirname(path)
    output_path = os.path.join(directory, 'Power.csv')
    power_plot_path = os.path.join(directory, 'PowerPlot')
    current_plot_path = os.path.join(directory, 'CurrentPlot')
    energy_plot_path = os.path.join(directory, 'EnergyPlot')
    power_overview_plot_path = os.path.join(directory, 'PowerOverviewPlot')
    energy_overview_plot_path = os.path.join(directory, 'EnergyOverviewPlot')

    print('Loading file %s' % (path,))
    data = pd.read_csv(path, sep=',', header=None)

    # Fix values
    data[SUPPLY_PROBE_LABEL] = data[SUPPLY_PROBE_LABEL] * -1
    data[SHUNT_PROBE_LABEL] = data[SHUNT_PROBE_LABEL] * -1
    data[WORKING_SIGNAL_PROBE_LABEL] = data[WORKING_SIGNAL_PROBE_LABEL] * -1
    data[COMPRESSING_SIGNAL_PROBE_LABEL] = data[COMPRESSING_SIGNAL_PROBE_LABEL] * -1

    # Drop Everything before the trigger
    start_index = data[data[TIME_LABEL] > 0].first_valid_index()
    data = data[start_index:].reset_index(drop=True)

    # Drop everything after the working signal goes down
    last_index = data[data[WORKING_SIGNAL_PROBE_LABEL] > 2].first_valid_index()
    data = data[:last_index].reset_index(drop=True)


    # Calculate power consumption
    voltage_drop = data[SHUNT_PROBE_LABEL]
    voltage_after_shunt = data[SUPPLY_PROBE_LABEL] - data[SHUNT_PROBE_LABEL]
    current = voltage_drop / SHUNT_VALUE
    total_power = current * voltage_after_shunt

    power_during_compression = []
    power_during_transmission = []
    energy_consumed_during_compression = []
    total_energy_consumed_during_compression = 0
    energy_consumed_during_transmission = []
    total_energy_consumed_during_transmission = 0
    total_energy_consumed = []
    compressing_flag = []

    last_time = 0
    last_power = total_power[0]
    total_energy = 0
    for row_index, row in data.iterrows():
        time = row[TIME_LABEL]
        power_at_time = total_power[row_index]
        average_power = (last_power + power_at_time) / 2
        time_passed = (time - last_time)
        energy_at_time = average_power * time_passed
        total_energy = total_energy + energy_at_time
        total_energy_consumed.append(total_energy)

        if row[COMPRESSING_SIGNAL_PROBE_LABEL] < 2:
            # Compressing at the time
            compressing_flag.append(1)
            power_during_compression.append(power_at_time)
            power_during_transmission.append(0)
            total_energy_consumed_during_compression = total_energy_consumed_during_compression + energy_at_time
            energy_consumed_during_compression.append(total_energy_consumed_during_compression)
            total_energy_consumed_during_transmission = total_energy_consumed_during_transmission + 0
            energy_consumed_during_transmission.append(total_energy_consumed_during_transmission)
        else:
            compressing_flag.append(0)
            power_during_compression.append(0)
            power_during_transmission.append(power_at_time)
            total_energy_consumed_during_compression = total_energy_consumed_during_compression + 0
            energy_consumed_during_compression.append(total_energy_consumed_during_compression)
            total_energy_consumed_during_transmission = total_energy_consumed_during_transmission + energy_at_time
            energy_consumed_during_transmission.append(total_energy_consumed_during_transmission)
        last_time = time
        last_power = power_at_time

    output = pd.DataFrame({
        OUTPUT_TIME_LABEL: data[TIME_LABEL],
        OUTPUT_CURRENT_LABEL: current,
        OUTPUT_VOLTAGE_LABEL: voltage_after_shunt,
        OUTPUT_POWER_USAGE_LABEL: total_power,
        OUTPUT_CONSUMED_ENERGY_LABEL: total_energy_consumed,
        OUTPUT_POWER_DURING_COMPRESSION_LABEL: power_during_compression,
        OUTPUT_POWER_DURING_TRANSMISSION_LABEL: power_during_transmission,
        OUTPUT_CONSUMED_ENERGY_DURING_COMPRESSION: energy_consumed_during_compression,
        OUTPUT_CONSUMED_ENERGY_DURING_TRANSMISSION: energy_consumed_during_transmission
    })

    output.to_csv(output_path, index=False)

    lgraph.plot_2d_dataframe(
            data_frame=output,
            x_column=OUTPUT_TIME_LABEL,
            y_columns=[OUTPUT_CURRENT_LABEL],
            file=current_plot_path,
            title=OUTPUT_CURRENT_LABEL)

    lgraph.plot_2d_dataframe(
        data_frame=output,
        x_column=OUTPUT_TIME_LABEL,
        y_columns=[OUTPUT_POWER_USAGE_LABEL],
        file=power_plot_path,
        title=OUTPUT_POWER_USAGE_LABEL)

    lgraph.plot_2d_dataframe(
        data_frame=output,
        x_column=OUTPUT_TIME_LABEL,
        y_columns=[OUTPUT_CONSUMED_ENERGY_LABEL],
        file=energy_plot_path,
        title=OUTPUT_CONSUMED_ENERGY_LABEL)

    lgraph.plot_2d_dataframe(
        data_frame=output,
        x_column=OUTPUT_TIME_LABEL,
        y_columns=[OUTPUT_POWER_USAGE_LABEL, OUTPUT_POWER_DURING_COMPRESSION_LABEL, OUTPUT_POWER_DURING_TRANSMISSION_LABEL],
        file=power_overview_plot_path,
        title='PowerOverview(W)')

    lgraph.plot_2d_dataframe(
        data_frame=output,
        x_column=OUTPUT_TIME_LABEL,
        y_columns=[OUTPUT_CONSUMED_ENERGY_LABEL, OUTPUT_CONSUMED_ENERGY_DURING_COMPRESSION,
                   OUTPUT_CONSUMED_ENERGY_DURING_TRANSMISSION],
        file=energy_overview_plot_path,
        title='EnergyOverview(W)')


    print('Done')


if __name__ == '__main__':
    main(sys.argv[1:])