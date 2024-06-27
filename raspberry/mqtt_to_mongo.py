#
# Copyright 2021 HiveMQ GmbH
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
import time
import paho.mqtt.client as paho
from paho import mqtt

# setting callbacks for different events to see if it works, print the message etc.
def on_connect(client, userdata, flags, rc, properties=None):
    print("CONNACK received with code %s." % rc)

# with this callback you can see if your publish was successful
def on_publish(client, userdata, mid, properties=None):
    print("mid: " + str(mid))

# print which topic was subscribed to
def on_subscribe(client, userdata, mid, granted_qos, properties=None):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

# print message, useful for checking if it was successful
def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))

def main() -> None:

	client = paho.Client(client_id="PythonPC", userdata=None, protocol=paho.MQTTv5)
	client.on_connect = on_connect
	client.tls_set(tls_version=mqtt.client.ssl.PROTOCOL_TLS)
	client.username_pw_set("progetto_tiai_antonio_lorenzo", "Concettocasoria2002")
	client.connect("e55a6c53127b400aa7c52f2e3c7a4ace.s1.eu.hivemq.cloud", 8883)

	client.on_subscribe = on_subscribe
	client.on_message = on_message
	client.on_publish = on_publish

	# i nostri topic sono del tipo /topic/temperature e /topic/humidity
	client.subscribe("topic/#", qos=1)

	client.loop_forever()


if __name__ == "__main__":
	main()