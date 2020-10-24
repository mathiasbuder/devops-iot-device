import sys
import os
import re
import numpy as np
import struct as st
import can.io.logger

class CanMessage:

    MaxLengthInBit = 64  # Maximum size of the message in bit

    # Constructor
    def __init__(self, name, CanId):
        self.CanId = CanId
        self.name = name
        self.CurrentLengthInBit = 0            # Current size of the message in bit
        self.signals            = []           # Empty signal list
        self.CanDataFrame_u64   = np.uint64(0) # Default/empty CAN message


    def __UpdateDataFrame(self):
        # Reset 64-bit CAN data frame to 0
        self.CanDataFrame_u64 = 0

        # Build frame from signal-tree
        for signal in self.signals:
            self.CanDataFrame_u64 |= signal['RawValueCasted'] << signal['StartBit']


    def __CreateBitMask(self, LengthInBit):
        # Init Bit-Mask
        BitMask = 0
        for BitPosition in range(LengthInBit):
            BitMask |= 1 << BitPosition
        return BitMask


    def AddSignal(self, name, signed, ScaleFactor, offset, LengthInBit, Startbit):
        SignalLength = self.CurrentLengthInBit + LengthInBit
        # Check for maximum message size
        if  self.MaxLengthInBit >= SignalLength :
            # signal = {'name': 'none' , 'type': 'True:signed / False:unsigned', 'ScaleFactor': 1.0, 'Offset': 0.0, 'LengthInBit': 0, 'StartBit': 0}
            self.signals.append({'name': name , 'signed': signed, 'ScaleFactor': ScaleFactor, 'offset': offset, 'LengthInBit': LengthInBit, 'StartBit': Startbit, 'BitMask': self.__CreateBitMask(LengthInBit), 'RawValueCasted': 0})
            self.CurrentLengthInBit += LengthInBit
            print('Signal {} added: Current size {}.' .format(name, self.CurrentLengthInBit))
        else:
            print('Signal {} not added: Maximum message size of {} > {} exceeded!' .format(name, SignalLength, self.MaxLengthInBit))
            return False
        return True


    def SetSignal(self, name, value):
        # Search for corresponding signal
        for signal in self.signals:
            if signal['name'] == name:
                # Convert signal according to scaling factor and offset
                RawVal = (1.0 / signal['ScaleFactor']) * (value - signal['offset'])
                # Cast raw value to corresponding integer
                if(True == signal['signed']):
                    if(signal['LengthInBit'] <= 8):
                        RawVal = np.int8(RawVal)
                    elif (signal['LengthInBit'] <= 16):
                        RawVal = np.int16(RawVal)
                    elif (signal['LengthInBit'] <= 32):
                        RawVal = np.int32(RawVal)
                    else:
                        RawVal = np.int64(RawVal)
                else:
                    if(signal['LengthInBit'] <= 8):
                        RawVal = np.uint8(RawVal)
                    elif (signal['LengthInBit'] <= 16):
                        RawVal = np.uint16(RawVal)
                    elif (signal['LengthInBit'] <= 32):
                        RawVal = np.uint32(RawVal)
                    else:
                        RawVal = np.uint64(RawVal)

                # Cast raw value to corresponding bit-length
                signal['RawValueCasted'] = signal['BitMask'] & RawVal

                # Build / Rebuild 64 bit message data frame
                self.__UpdateDataFrame()
                return True

        print('{}: Signal {} not found!' .format(self.name, name))


    def GetId(self):
        return self.CanId


    def GetSignals(self):
        return self.signals


    def GetSizeInBit(self):
        return self.CurrentLengthInBit


    def PrintSignals(self):
        print('{} signals:'.format(self.name))
        for signal in self.signals:
            print('Name: {}, Signed: {}, ScaleFactor: {}, LengthInBit: {}, StartBit: {}, BitMask: {}' .format(signal['name'], signal['signed'], signal['ScaleFactor'], signal['LengthInBit'], signal['StartBit'], np.binary_repr(signal['BitMask'], width=self.MaxLengthInBit) ))


    def PrintSignalLayout(self):
        print('{} signal layout:'.format(self.name))
        for signal in self.signals:
            print('{0:>30}: {1:>64}' .format(signal['name'],  np.binary_repr( (signal['BitMask'] << signal['StartBit'] ), width=self.MaxLengthInBit)))
 

    def GetDataFrame(self, PrintFrame=False):
        if(True == PrintFrame):
            print('{0:>30}: {1:>64}'.format('Data frame', np.binary_repr( self.CanDataFrame_u64, width=self.MaxLengthInBit)))
        return self.CanDataFrame_u64



DD_INA_219_Data_A = CanMessage('DD_INA_219_Data_A', 30)
DD_INA_219_Data_B = CanMessage('DD_INA_219_Data_B', 31)

DD_INA_219_Data_A.AddSignal('DD_INA_219_bus_voltage_raw',     False, 1, 0.0, 16, 0)
DD_INA_219_Data_A.AddSignal('DD_INA_219_shunt_voltage_raw',   False, 1, 0.0, 16, 16)
DD_INA_219_Data_A.AddSignal('DD_INA_219_current_raw',         False, 1, 0.0, 16, 32)
DD_INA_219_Data_A.AddSignal('DD_INA_219_power_raw',           False, 1, 0.0, 16, 48)
DD_INA_219_Data_A.PrintSignals()
DD_INA_219_Data_A.PrintSignalLayout()

DD_INA_219_Data_A.SetSignal('DD_INA_219_bus_voltage_raw', 10)
DD_INA_219_Data_A.SetSignal('DD_INA_219_current_raw', 16)

DD_INA_219_Data_A.GetDataFrame(True)



DD_INA_219_Data_B.AddSignal('DD_INA_219_current',             False, 0.1, 0.0, 14, 0)
DD_INA_219_Data_B.AddSignal('DD_INA_219_shunt_voltage_range', False, 1,   0.0, 2,  14)
DD_INA_219_Data_B.AddSignal('DD_INA_219_power',               False, 0.1, 0.0, 14, 16)
DD_INA_219_Data_B.AddSignal('DD_INA_219_bus_voltage_range',   False, 1,   0.0, 1,  30)
DD_INA_219_Data_B.AddSignal('DD_INA_219_shunt_voltage',       False, 0.1, 0.0, 9,  32)
DD_INA_219_Data_B.AddSignal('DD_INA_219_bus_voltage',         False, 0.1, 0.0, 8,  48)

DD_INA_219_Data_B.PrintSignals()
DD_INA_219_Data_B.PrintSignalLayout()

DD_INA_219_Data_B.SetSignal('DD_INA_219_current', 234.5)
DD_INA_219_Data_B.SetSignal('DD_INA_219_bus_voltage', 10)

DD_INA_219_Data_B.GetDataFrame(True)
























#message1 = CanMessage(31)

#message1.AddSignal('signal_1', False, 0.1, 0.0, 4, 2)
#message1.AddSignal('signal_2', False, 2.0, 1.0, 54, 5)
#message1.AddSignal('signal_3', False, 2.0, 1.0, 1, 5)

#message1.SetSignal('signal_1', 1.5)

#message1.PrintSignals()

#CanData = message1.GetDataFrame()

#print('{}' .format(np.binary_repr(CanData, width=64)))

#for signal in message1.signals:
#    print('Name: {}, Type: {}, ScaleFactor: {}, LengthInBit: {}, StartBit: {}, BitMask: {}' .format(signal['name'], signal['type'], signal['ScaleFactor'], signal['LengthInBit'], signal['StartBit'], np.binary_repr(signal['BitMask'], width=64) ))