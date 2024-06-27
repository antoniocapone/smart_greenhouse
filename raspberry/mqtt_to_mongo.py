import sqlite3, time
import paho.mqtt.client as paho
from paho import mqtt

# conn = sqlite3.connect("progetto_tiai.db")

def on_connect(client, userdata, flags, rc, properties=None):
    print("CONNACK received with code %s." % rc)

def on_publish(client, userdata, mid, properties=None):
    print("mid: " + str(mid))

def on_subscribe(client, userdata, mid, granted_qos, properties=None):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))

def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
    # try:
    #     insert_query = """
    #         INSERT INTO MISURAZIONI (tipo, valore, data)
    #         VALUES ('%s', %d, %d);
    #     """
    #     print(f"Eseguo la query: {insert_query % (str(msg.topic), int(msg.payload.decode("utf-8").encode("unicode-escape").decode("unicode-escape")), time.time())}")
    #     conn.execute(insert_query % (str(msg.topic), int(msg.payload.decode("utf-8").encode("unicode-escape").decode("unicode-escape")), time.time()))
    #     conn.commit()
    # except sqlite3.OperationalError as e:
    #     print(f"Errore query: {e}")
    # except ValueError:
    #     pass

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
	client.subscribe("/topic/#", qos=0)

	client.loop_forever()


if __name__ == "__main__":
	main()