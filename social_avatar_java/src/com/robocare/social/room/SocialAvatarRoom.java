package com.robocare.social.room;

import com.robocare.front.message.GameMessage;
import com.robocare.front.server.room.Room;
import com.robocare.front.server.room.RoomContainer;
import com.robocare.rosjava.util.listener.UtilityMessageListener;
import com.robocare.rosjava.util.listener.UtilityMessageListenerFactory;

public class SocialAvatarRoom extends Room implements UtilityMessageListener{

	public SocialAvatarRoom() {
		super("SOCIAL_AVATAR");
	}
	
	public void ready() {
		UtilityMessageListenerFactory.getInstance().addListener(this);
	}
	

	public void end() {
		UtilityMessageListenerFactory.getInstance().removeListener(this);
	}

	@Override
	public void service(GameMessage reqMessage, GameMessage resMessage) {
		
		String op = reqMessage.getOperationName();
		if(op.equals("req_noti_set_image")) {
			resMessage.setOperationName("res_noti_set_image");
			String face = reqMessage.getParameterValue("id");
			GameMessage faceMessage = new GameMessage();
			faceMessage.setOperationName("noti_set_image");
			faceMessage.addParameter("id", "face-"+face);
			faceMessage.addParameter("param", face);
			RoomContainer.getInstance().getCurrentRoom().broadcast(faceMessage);
		} else if(op.equals("req_flash_ready")) {
			resMessage.setOperationName("res_flash_ready");
		}
	}

	@Override
	public void handleMessage(String arg0) {
		
	}
	

}