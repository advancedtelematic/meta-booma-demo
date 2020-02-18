#!/usr/bin/env python3

from rplidar import RPLidar
import sys
import numpy as np
import time
import json
from datetime import datetime

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
		print('Recording measurments... Press Ctrl+C to stop.')
		for measurment in lidar.iter_measures():
			line = '\t'.join(str(v) for v in measurment)
			outfile.write(line + '\n')
			print(line)
	except KeyboardInterrupt:
		print('Stopping.')
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

def print_json():
	lidar = RPLidar(PORT_NAME)
	data = {}
	try:
		sys.stderr.write('Recording measurments... Press Ctrl+C to stop.\n\nCompleted scan ')
		for i, scan in enumerate(lidar.iter_scans(max_buf_meas=25000)):
			if i >= 10:
				sys.stderr.write('Stopping after 10 scans.\n')
				break
			data[datetime.utcnow().strftime("%Y_%m_%d_%H_%M_%S.%f")[:-3]] = scan
			sys.stderr.write(str(i+1) + '...')
			sys.stderr.flush()
	except KeyboardInterrupt:
		sys.stderr.write('Stopping.\n')
	lidar.stop()
	lidar.stop_motor()
	lidar.disconnect()
	print(str(json.dumps(data)))


if len(sys.argv) < 2:
	print("\nInvalid parameter")
	print("\nUsage: booma-client.py <option>\n")
elif sys.argv[1] == "--check":
	health_check()
elif sys.argv[1] == "--measure":
	measure()
elif sys.argv[1] == "--scan":
	scan()
elif sys.argv[1] == "--json":
	print_json()
else:
	print("Incorrect option selected, please choose one from\n(1) check\n(2) scan\n(3) measure")