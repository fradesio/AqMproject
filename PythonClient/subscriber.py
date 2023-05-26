import paho.mqtt.client as mqtt

broker_address = "192.168.10.101"
broker_port = 1883
mqtt_topic = "sensors_data"


def on_message(client, userdata, message):
    print("Received message: " + str(message.payload.decode()))


client = mqtt.Client()
client.on_message = on_message

# Connect to the broker
client.connect(broker_address, broker_port)

# Subscribe to the MQTT topic
client.subscribe(mqtt_topic)

# Start the MQTT loop to handle incoming messages
client.loop_start()

# Keep the script running to receive messages
while True:
    pass
