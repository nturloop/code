mqClient = mqttclient("mqtt://192.168.0.145",Port=1883)
    topicToSub = "ESP32/MPPT/TensionIN";
    subscribe(mqClient, topicToSub)
    subscribe(mqClient, "ESP32/MPPT/TensionOUT")

    dataTT = read(mqClient)

    clear mqClient