package com.robocare.social;

import java.io.IOException;

import com.robocare.front.server.GameServer;
import com.robocare.front.server.room.RoomContainer;
import com.robocare.rosjava.RosNodeExecutor;
import com.robocare.social.room.SocialAvatarRoom;
import com.robocare.social.test.SocialAvatarTestClient;
import com.robocare.social.util.SocialAvatarUtility;
import com.robocare.utility.config.Config;

public class SocialAvatar {

	public SocialAvatar() {
		System.out.println("SocialAvatar ver 0.1.1");
		initServer();
		RosNodeExecutor.execute(SocialAvatarUtility.getInstance());
		
	}

	public void initServer() {

		new Thread(new Runnable()
		{
			@Override
			public void run()
			{
				GameServer server = new GameServer();
				RoomContainer.getInstance().addRoom("SOCIAL_AVATAR", new SocialAvatarRoom());

				server.start();
				System.out.println("---server start---");
			}
		}).start();

		try {
			Thread.sleep(3000L);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		RoomContainer.getInstance().changeCurrentRoom("SOCIAL_AVATAR");
		
		if(Config.getParamValueInt("game.server.test") == 1) {
			new SocialAvatarTestClient().init();
		}
		
		try {
			System.out.println("LOAD AVATAR rosrun social_avatar load_avatar");
			Runtime.getRuntime().exec("rosrun social_avatar load_avatar");
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("End of Social Avatar");
//		try {
//			Thread.sleep(3000L);
//		} catch (InterruptedException e) {
//			e.printStackTrace();
//		}
//		try {
//			Runtime.getRuntime().exec("./system/data/shell/move_avatar");
//		} catch (IOException e) {
//			e.printStackTrace();
//		}
	}

	public static void main(String[] args) {
		new SocialAvatar();
	}
}
