	class LibMenu 
	{
		private:
			u8					mId;
			LibPos 				mPos;
			std::string 		mTitle;
			std::string 		mContent;
			LibColor	mForecolor, mBackcolor;

			int			mMenuItemSpacing = 10;
			int			mMenuItemCount;
			float		mMenuItemSelection;

			std::map<int,std::string>			mMenuItems;
			std::vector<std::function<void()>> 	mMenuItemCallbacks;
			std::array<bool, 16>	mMenuItemsSelected;

			bool bMenuIsShowing   = true;
			bool bEnableHighlight = true;
			bool bKeyStateHeld    = true;
			LibColor cHighlightColor = LibColor::RED;
		public:
			bool 		bInFocus;		
		public:
			LibMenu(u8 i, std::string t, LibPos p, LibColor f = BLACK, LibColor b = WHITE)
			{ 
				mId = i; mPos = p; mTitle = t; mForecolor = f; mBackcolor = b;
			}

			template<class FunctionCallback>
			void AddMenuItem(int mId, std::string content, FunctionCallback call = [](){}) 
			{
				if(mMenuItems.size() <= 16) 
				{
					mMenuItems[mId] = content;
					mMenuItemCallbacks.push_back(call);
					mMenuItemCount++;
				}
			}

			void AddMenuItem(int mId, std::string content) 
			{
				if(mMenuItems.size() <= 16) 
				{
					mMenuItems[mId] = content;
					mMenuItemCallbacks.push_back([](){});
					mMenuItemCount++;
				}
			}

			void MoveSelectionUp(Frame &r)   
			{ 
				mMenuItemsSelected.fill(false);

				if((mMenuItemSelection - 1) >= 0 
				&& bInFocus && MenuIsShowing()) 
					mMenuItemSelection-= 1; 
			}

			void MoveSelectionDown(Frame &r) 
			{ 
				mMenuItemsSelected.fill(false);

				if((mMenuItemSelection + 1) < mMenuItemCount
				 && bInFocus && MenuIsShowing()) 
					mMenuItemSelection+= 1; 
			}

			void Show()
			{
				if(bMenuIsShowing) 
				{
					auto findLargestMenuItem = [&]() -> int
					{
						std::vector<int> stringLengths;
						for(auto& t : mMenuItems) {
							stringLengths.push_back(t.second.length());
						}
						stringLengths.push_back(this->mTitle.length());

						std::sort(stringLengths.begin(), stringLengths.end(), [](int a, int b) {
							return a > b;
						});

						return stringLengths.front();
					};
					
					auto toUpper = [this](std::string str) -> std::string
					{
						std::string tmp;
						for(auto& character : str) {
							tmp += toupper(character);
						}
						return tmp;
					};

					LibPos dimensions = 
					{ 
						10 * findLargestMenuItem(), 
						35 + (10 * mMenuItemCount)
					};
					
					
					Display::DrawRect(mPos, dimensions.x, dimensions.y, mForecolor);
					Display::DrawRect(mPos, dimensions.x, dimensions.y, mBackcolor, false);
		
					Display::DrawRect(mPos, dimensions.x, 15, mBackcolor);
					Display::SetColors(mForecolor, mBackcolor);
					Display::DrawText({mPos.x+5, mPos.y+5}, mTitle);
					Display::SetColors(GOLD, NAVY_BLUE | 0xFF);

					int incy = 20, spot = 0;
					for(auto &t : mMenuItems) 
					{
						if(bInFocus && (int)mMenuItemSelection == spot) 
						{
							if(bEnableHighlight) 
							{
								Display::DrawRect({mPos.x+2, mPos.y+incy-2}, dimensions.x-3, 10, cHighlightColor);
							}

							Display::DrawText({mPos.x+5, mPos.y+incy}, toUpper(t.second));
						} 
						else 
							Display::DrawText({mPos.x+5, mPos.y+incy}, t.second);
						incy+=mMenuItemSpacing;
						++spot;
					}
				}
			}

			bool MenuItemIsSelected(int item) 
			{
				return mMenuItemsSelected[item];
			}

			bool MenuIsShowing() {
				return bMenuIsShowing;
			}

			void Hide() {
		
				bMenuIsShowing = false;
				SetUnfocused();
			}

			void EnableShow() {
				bMenuIsShowing = true;
			}

			void EnableHighlight() {
				bEnableHighlight = true;
			}

			void DisableHighlight() {
				bEnableHighlight = false;
			}

			void SetHighlightColor(LibColor c) {
				cHighlightColor = c;
			}

			void WaitKeyPress() {
				if(bInFocus)
				{
					mMenuItemsSelected.at(mMenuItemSelection) = true;
					mMenuItemCallbacks.at(mMenuItemSelection)();
				}
			}
			void SetItemSpacing(int spacing) {
				mMenuItemSpacing = spacing;
			}

			void SetFocused() {
				bInFocus = true;
			}

			void SetUnfocused() {
				bInFocus = false;
			}
	};

	class LibMenuManager
	{
		private:
			std::vector     <LibMenu*>  menuList;
			std::map    <u32, LibMenu*>  menuMap;

		public:
			void AddMenu(u32 i, std::string t, LibPos p, LibColor f, LibColor b)
			{
				/*menuList.push_back(new LibMenu(i, t, p, f, b));
				menuMap[i] = menuList.back();*/

				auto *tmp = new LibMenu(i, t, p, f, b);
				menuList.push_back(tmp);
				menuMap[i] = tmp;
			}

			bool AllMenusClosed() 
			{
				for(auto &menus : menuList) {
					if(menus->MenuIsShowing() || menus->bInFocus) {
						return false;
					}
				}
				return true;
			}

			void CloseFocusedMenus() 
			{
				for(auto& menus : menuList) {
					if(menus->bInFocus) {
						menus->Hide();
					}
				}
			}

			void CloseAllMenus() 
			{
				for(auto& menus : menuList) {
					menus->Hide();
				}
			}

			LibMenu* operator [](u32 i) { return menuMap[i]; }
	};