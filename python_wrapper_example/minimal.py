import ctypes
import json
import time
lo = ctypes.CDLL("./libliveobjects.so")
lo.LiveObjectsClient_SetDevId("python")
lo.LiveObjectsClient_Init(None, ctypes.c_longlong(int("0xb86b1cbdf6064e2a",0)), ctypes.c_longlong(int("0xaa82d97ec988f4e2",0)))
lo.LiveObjectsClient_Connect()

while True:
	lo.LiveObjectsClient_Publish("dev/data",ctypes.c_char_p(json.dumps({"streamId":"python_test","value":{"python test":"python test"}})))
	time.sleep(1)


lo.LiveObjectsClient_Disconnect()
