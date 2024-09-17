function SendCTRLData(MqttClient,DataCTRState)

%DutyCycleCTRTopic = "ESP32/MPPT/DutyCyleCTR";
%OCPINTopic = "ESP32/MPPT/OCPINT_State";
%BackFlowTopic ="ESP32/MPPT/BackFlow";
%PinSubTopic = "ESP32/MPPT/PinSub";
%CCProtTopic = "ESP32/MPPT/CCProt";
%IR2184Topic = "ESP32/MPPT/IR2184";

JSONDATATopic = "ESP32/MPPT/JSONDATACTR";

%write(MqttClient,DutyCycleCTRTopic,sprintf('%.2f',DataCTRState.DutyCyleCTR))
%write(MqttClient,OCPINTopic,sprintf('%d',DataCTRState.OCPINState))
%write(MqttClient,BackFlowTopic,sprintf('%d',DataCTRState.BackFlowState))
%write(MqttClient,PinSubTopic,sprintf('%d',DataCTRState.PinSubsate))
%write(MqttClient,CCProtTopic,sprintf('%d',DataCTRState.CCProtState))
%write(MqttClient,IR2184Topic,sprintf('%d',DataCTRState.IR2184State))

JSONDataCTR = jsonencode(DataCTRState,PrettyPrint=true)

write(MqttClient,JSONDATATopic,JSONDataCTR)

end