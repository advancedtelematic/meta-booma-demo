#!/usr/bin/env python3

from rplidar import RPLidar
import sys
import numpy as np
import time
import datetime

PORT_NAME = '/dev/ttyUSB0'
time_stamp = time.strftime("%Y_%m_%d_%H_%M_%S", time.gmtime())

def health_check():
	lidar = RPLidar(PORT_NAME)
	info = lidar.get_info()
	print(info)

	health = lidar.get_health()
	print(health)

	for i, scan in enumerate(lidar.iter_scans()):
		print('%d: Got %d measurments' % (i, len(scan)))
		if i > 10:
			break

	lidar.stop()
	lidar.stop_motor()
	lidar.disconnect()
	return

def measure():
	lidar = RPLidar(PORT_NAME)
	path = sys.argv[2] if len(sys.argv) >= 3 else 'lidar_measurements_' + time_stamp + '.log'
	outfile = open(path, 'w')
	try:
		print('Recording measurments... Press Crl+C to stop.')
		for measurment in lidar.iter_measures():
			line = '\t'.join(str(v) for v in measurment)
			outfile.write(line + '\n')
			print(line)
	except KeyboardInterrupt:
		print('Stoping.')
	lidar.stop()
	lidar.stop_motor()
	lidar.disconnect()
	outfile.close()
	return

def scan():
	lidar = RPLidar(PORT_NAME)
	data = []
	try:
		print('Recording measurments... Press Crl+C to stop.')
		for scan in lidar.iter_scans():
			data.append(np.datetime64(datetime.datetime.now()))
			data.append(np.array(scan))
			print(scan)
	except KeyboardInterrupt:
		print('Stoping.')
	lidar.stop()
	lidar.stop_motor()
	lidar.disconnect()
	path = sys.argv[2] if len(sys.argv) >= 3 else 'lidar_scan_' + time_stamp +'.log'
	np.save(path, np.array(data))

if len(sys.argv) < 2:
	print("\nInvalid parameter")
	print("\nUsage: booma-client.py <option>\n")
elif sys.argv[1] == "--check":
	health_check()
elif sys.argv[1] == "--measure":
	measure()
elif sys.argv[1] == "--scan":
	scan()
else:
	print("Incorrect option selected, please choose one from\n(1) check\n(2) scan\n(3) measure")