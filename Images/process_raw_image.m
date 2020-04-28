% Impoirt the image
imput_file = 'C:\Users\Laurynas\Desktop\Wireless-Sensor-Networks-Mini-Project\Images\Lena\lena_gray.raw';
output_file = 'C:\Users\Laurynas\Desktop\Wireless-Sensor-Networks-Mini-Project\Images\Lena\lena.txt';
original_rows = 512;
original_columns = 512;
scale = 0.5;
file = fopen(imput_file, 'r');
image = fread(file, original_rows * original_columns, 'uint8=>uint8');
image = reshape(image, original_rows, original_columns);
image = image';
image = imresize(image, scale);

image(image == 255) = 254;

dlmwrite(output_file, image);
imshow(image);
