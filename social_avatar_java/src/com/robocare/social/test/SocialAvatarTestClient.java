package com.robocare.social.test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.robocare.front.ui.ExtraUIData;
import com.robocare.front.ui.GameExtraPanel;
import com.robocare.front.ui.GameExtraUI;

public class SocialAvatarTestClient {

	public void init() {
		String name = "SocialAvatarTestClient";
		Map<String, GameExtraPanel> messagePanelMap = new HashMap<String, GameExtraPanel>();
		
		List<ExtraUIData> mainMenuDataList = new ArrayList<ExtraUIData>();
				
		ExtraUIData image = new ExtraUIData("noti_set_image", "req_noti_set_image");
		image.addParam("id");
		mainMenuDataList.add(image);

		GameExtraPanel mainMenu = new GameExtraPanel(mainMenuDataList);
		messagePanelMap.put("noti_set_image_main_menu", mainMenu);
		
		GameExtraUI ui = new GameExtraUI(name, messagePanelMap);
		
		ui.setInitialPanel(mainMenu);
	}
	
	public static void main(String[] args) {
		new SocialAvatarTestClient().init();
	}
}
