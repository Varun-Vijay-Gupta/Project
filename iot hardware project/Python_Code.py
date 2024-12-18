import serial
import time

# Open the serial port (adjust 'COM3' for your system)
ser = serial.Serial('COM3', 115200, timeout=1)  # Replace 'COM3' with your port
time.sleep(2)  # Wait for the serial connection to establish

image_data = []  # List to store image data
total_pixels_received = 0  # Initialize pixel count
capturing_image_started = False  # Flag to check if capturing has started

print("Waiting for image data...")

while True:
    line = ser.readline().decode('utf-8').strip()  # Read line from Arduino

    if "Capturing image..." in line:
        capturing_image_started = True  # Set the flag when image capturing starts
        continue  # Skip this line

    if line == "Image data sent.":
        # Stop receiving data when the image transmission is complete
        break

    if capturing_image_started:  # Only process lines after capturing starts
        if line:  # If line contains pixel data
            image_data.append(line)  # Store the pixel data line
            total_pixels_received += len(line.split(','))  # Count the pixels in the line

# Close the serial port
ser.close()

# Save the image data to a file
with open("captured_image.txt", "w") as f:
    for data in image_data:
        f.write(data + "\n")

# Output the results
print("Image data received.")
print(f"Total pixels received: {total_pixels_received}")
print("Image data saved to captured_image.txt")
