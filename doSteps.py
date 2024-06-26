import json
from vmipy.vmichannel import MqttChannel
from vmipy.vmilog import getLogger, LogLevelEnum
import threading
import queue
import time

class OHand:
    def __init__(self, host="localhost", port=1883, 
                 username="mqtt:mqtt-test",
                 password="mqtt-test", 
                 deviceType="ohand",
                 deviceId="001", 
                 uartChannel=None):
        self._deviceType = deviceType
        self._deviceId = deviceId
        self._brokerHost = host
        self._brokerPort = port
        self._brokerUser = username
        self._brokerPwd = password
        self._uartChannel = uartChannel
        self._connectEvent = threading.Event()
        self._channel = MqttChannel(host=host,
                                    port=port,
                                    username=username,
                                    password=password)
        self._channel.on_connect = self._on_connected
        self._channel.on_disconnect = self._on_disconnect
        self._channel.on_message = self._on_message
        self._client = None
        self._connectEvent.clear()
        self._logger = getLogger(logName="{}_{}".format(self._deviceType, self._deviceId),
                                 enableConsole=True,
                                 logLevel=LogLevelEnum.info)
        self._queue = queue.Queue()
        self._MQTTID = 0
        self._c2dTopic = "c2d/{}/{}/service".format(self._deviceType, self._deviceId)

    def _on_connected(self, client, userdata, flags, rc):
        self._logger.info("{},{} connected".format(self._deviceType, self._deviceId))
        self._client = client
        self._connectEvent.set()
        #self._client.subscribe(self._services)

    
    def _on_disconnect(self, client, userdata, rc):
        self._client = None
        self._connectEvent.clear()
    
    def _on_message(self, client, userdata, message):
        self._logger.info("topic: {topic}, msg: {msg}".format(topic=message.topic, msg=message.payload))
        try:
            obj = json.loads(message.payload)
            topic = message.topic
        except:
            self._logger.error("json loads payload error")
            return
        else:
            self.mockCommand(obj)
    
    def mockCommand(self, req):
        pass

    def Run(self):
        self._channel.RunAsync()
        while True:
            fingersParas = self._queue.get()
            if fingersParas == "quit":
                break
            request = {
                "method": "thing.service.finger.set",
                "inputData": {
                  "fingers": fingersParas  
                }
            }
            requestStr = json.dumps(request)
            c2dCmd = {
                "Topic": "c2d/{}/{}/service".format(self._deviceType, self._deviceId),
                "MqttID": self._MQTTID,
                "Request": requestStr
            }
            if self._connectEvent.is_set():
                self._client.publish(topic=self._c2dTopic,payload=json.dumps(c2dCmd),qos=0)

    def setFingers(self, paras):
        self._queue.put(paras)

class Configuration:
    ohands = [
        {"deviceType": "ohand", "deviceId": "001"},
        {"deviceType": "ohand", "deviceId": "002"},
        {"deviceType": "ohand", "deviceId": "003"},
        {"deviceType": "ohand", "deviceId": "004"},
        {"deviceType": "ohand", "deviceId": "005"},
        {"deviceType": "ohand", "deviceId": "006"},
        {"deviceType": "ohand", "deviceId": "007"},
        {"deviceType": "ohand", "deviceId": "008"},
    ]
    broker = "localhost"
    broker_port = 1883

def main():
    ohandObj = {}
    ohandThreads =[]
    for conf in Configuration.ohands:
        ohandType = conf["deviceType"]
        ohandId = conf["deviceId"]
        ohandKey = "{}_{}".format(ohandType, ohandId)
        ohandObj[ohandKey] = OHand(host=Configuration.broker, port=Configuration.broker_port,deviceType=ohandType, deviceId=ohandId)
        t = threading.Thread(target=ohandObj[ohandKey].Run, daemon=True)
        t.start()
        ohandThreads.append(t)
    with open("steps.json") as steps_file:
        file_contents = steps_file.read()
    parsed_stpes = json.loads(file_contents)
    steps = parsed_stpes["steps"]
    while True:
        for thisStep in steps:
            if thisStep["action"] == "setfingers":
                params = thisStep["params"]
                for param in params:
                    ohanid = "{}_{}".format(param["deviceType"], param["deviceId"])
                    ohand = ohandObj.get(ohanid)
                    if ohand is not None:
                        ohand.setFingers(param["value"])
            elif thisStep["action"] == "sleep":
                sleepDuration = thisStep["params"]
                time.sleep(sleepDuration)
    

if __name__ == "__main__":
    main()




