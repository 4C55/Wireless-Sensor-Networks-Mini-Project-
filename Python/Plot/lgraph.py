import plotly.graph_objects as go
from plotly.offline import plot


def get_plot_of_2d_dataframe(
        data_frame,
        x_column,
        y_columns=None,
        y_skip_columns=None,
        x_log=False,
        title=None):
    fig = go.Figure()
    x = data_frame[x_column]
    ys = data_frame.drop([x_column], axis=1)
    if y_columns is not None:
        ys = ys[y_columns]
    if y_skip_columns is not None:
        ys = ys.drop(y_skip_columns, axis=1)
    for y in ys:
        fig.add_trace(go.Scatter(
            x=x,
            y=ys[y],
            mode='lines',
            name=y
        ))

    if x_log:
        fig.update_layout(xaxis_type="log")

    if title is not None:
        fig.update_layout(
            title=title
        )

    return fig


def save_plot(fig, file):
    plot(fig, filename=file)


def plot_2d_dataframe(
        data_frame,
        x_column,
        y_columns=None,
        y_skip_columns=None,
        x_log=False,
        file=None,
        title=None):
    fig = get_plot_of_2d_dataframe(data_frame, x_column, y_columns, y_skip_columns, x_log, title)

    if file is None:
        fig.show()
    else:
        save_plot(fig, file)

    return fig


def add_text_annotations(fig, locations, text):
    annotations = []
    for index, location in enumerate(locations):
        annotations.append(
            go.layout.Annotation(
                x=location[0],
                y=location[1],
                xref="x",
                yref="y",
                text=text,
                showarrow=True,
                arrowhead=7,
                ax=0,
                ay=-40))
    fig.update_layout(
        annotations=annotations
    )

    return fig