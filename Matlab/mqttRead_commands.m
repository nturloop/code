function newMessage = mqttRead_commands()
    % Define persistent variables
    % Persistend, because they need to stay valid between function calls
    persistent mqttClient
    persistent outMessage
    % Initialize MQTT client and subscribe to a topic
    % Warning: There still is a debricated Function called mqtt() that can
    % be used for Generating a MQTT Client. Use the function mqttclient(),
    % it is the new still supported version.
    if isempty(mqttClient) % To execute the code only once
        mqttClient = mqttclient('mqtt://192.168.0.145', Port=1883); %tcp:// because the client and broker are connected via ethernet
        mySub = subscribe(mqttClient, "ESP32/MPPT/TensionIN") % write the topic string directly into the parameter. No Variables.
        outMessage = "NoOutput";
    end
    
    % Empfangen der MQTT Nachricht
    % The "Topic=" is on purpose. The Syntax between read and write commands is differently. 
    % This is also confirmed by the Mathworks Support.
    % Keep the syntax exactly the same.
    mqttMsg = read(mqttClient)
    
    % Sending of a MQTT Message to a Dead Topic to keep Client alive.
    % Otherwise the Client will shutdown after 50-60 seconds.
    % Keep the syntax exactly the same.
    mqttMsg_writeTest = "DeadMessageString";
    write(mqttClient, "deadTopic/keepClientAlive", mqttMsg_writeTest);
    
    if isempty(mqttMsg)
        % Case for if no message is received in that topic.
        disp("mqttMsg is empty");
    else
        % Converts the mqtt Message JSON String into a struct.
        % "*.Data(end)" attachment chooses always the last Data of the last received message.
        mqttMsg_struct = jsondecode(mqttMsg.Data(end));
    end
    % Transfer received Message JSON String to Output Parameter 
    % "newMessage" if a MQTT Message was received.
    if isempty(mqttMsg)
        newMessage = outMessage;
    else
        newMessage = mqttMsg.Data(end)
        outMessage = newMessage
    end 
end