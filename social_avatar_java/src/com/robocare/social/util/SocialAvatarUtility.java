package com.robocare.social.util;

import org.ros.message.MessageListener;
import org.ros.namespace.GraphName;
import org.ros.node.AbstractNodeMain;
import org.ros.node.ConnectedNode;
import org.ros.node.topic.Subscriber;

import com.robocare.front.message.GameMessage;
import com.robocare.front.server.room.RoomContainer;

public class SocialAvatarUtility extends AbstractNodeMain{

	private Subscriber<std_msgs.String> eventSubscriber = null;
	
	private static SocialAvatarUtility __instance = new SocialAvatarUtility();
	public static SocialAvatarUtility getInstance() {
		return __instance;
	}
	
	
	private SocialAvatarUtility() {
	}
	
	public void onStart(final ConnectedNode connectedNode) {
		
		eventSubscriber = connectedNode.newSubscriber("/social_avatar/face",std_msgs.String._TYPE);
		eventSubscriber.addMessageListener(new MessageListener<std_msgs.String>() {
			
			@Override
			public void onNewMessage(std_msgs.String msg) {
				String face = msg.getData();
				GameMessage faceMessage = new GameMessage();
				faceMessage.setOperationName("noti_set_image");
				faceMessage.addParameter("id", "face-"+face);
				RoomContainer.getInstance().getCurrentRoom().broadcast(faceMessage);
			}
		});
	}
		
	@Override
	public GraphName getDefaultNodeName() {
		return GraphName.of("DementisUIUtility"+System.currentTimeMillis());
	}

	
	
}
