#!/usr/bin/python

import smbus
import time
bus = smbus.SMBus(1)    # 0 = /dev/i2c-0 (port I2C0), 1 = /dev/i2c-1 (port I2C1)

DEVICE_ADDRESS = 0x0F
DEVICE_REG = 0x1d

requisition = "{\"is_locked\":1, \"date\":\"17/11/2017\", \"temperature\":\"2.0\", \"longitude\":\"222.78\", \"enable\":1, \"latitude\":\"-78.98\", \"transport_id\":50} "
requisition_data = requisition.split(" ",7)
data_array = []

def changeToBytes(string):
	data_array = []
	for char in string:
		data_array.append(ord(char[0]))
	return data_array

x = 0
while (x < 7):
	data = changeToBytes(requisition_data[x])
	bus.write_i2c_block_data(DEVICE_ADDRESS, DEVICE_REG, data)
	time.sleep(1)
	print data
	x = x+1
