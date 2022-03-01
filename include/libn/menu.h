#ifndef LIBN_MENU_H
#define LIBN_MENU_H
	
#include <libn.h>

typedef struct
{
		u8			mId;
		LibPos		mPos;
		char* mTitle;
		char* mContent;
		LibColor mForecolor, mBackcolor;

		u32	mMenuItemSpacing = 10;
		u32	mMenuItemCount;
		f32	mMenuItemSelection;

		std::map<u32,std::string>			mMenuItems;
		void (*callbacks)()[25];
		std::vector<std::function<void()>> 	mMenuItemCallbacks;
		bool mMenuItemsSelected[16];

		bool bMenuIsShowing   = true;
		bool bEnableHighlight = true;
		bool bKeyStateHeld    = true;
		LibColor cHighlightColor;
		bool 		bInFocus;	
} LibMenu;	

void LibMenu_Init(LibMenu* menuRef, const u8 ID, const char* text, const LibPos Pos, LibColor foreground, LibColor background) {
	menuRef->mId = ID;
	menuRef->mPos = Pos;
	menuRef->mTitle = text;
	menuRef->mForecolor = foreground;
	menuRef->mBackcolor = background;
}

void LibMenu_AddMenuItem(LibMenu* menuRef, const u8 submID, const char* content, void(*callback)())
{
	
}
		void AddMenuItem(const u32 mId, const char* content, const std::function<void()> callback) 
		{
			if(mMenuItems.size() <= 16) 
			{
				mMenuItems[mId] = content;
				mMenuItemCallbacks.push_back(callback);
				mMenuItemCount++;
			}
		}

		void AddMenuItem(u32 mId, std::string content) 
		{
			if(mMenuItems.size() <= 16) 
			{
				mMenuItems[mId] = content;
				mMenuItemCallbacks.push_back([](void){});
				mMenuItemCount++;
			}
		}

		void MoveSelectionUp()   
		{ 
			mMenuItemsSelected.fill(false);

			if((mMenuItemSelection - 1) >= 0 
			&& bInFocus && MenuIsShowing()) 
				mMenuItemSelection-= 1; 
		}

		void MoveSelectionDown() 
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
				auto findLargestMenuItem = [&]() -> u32
				{
					std::vector<u32> stringLengths;
					for(auto& t : mMenuItems) {
						stringLengths.push_back(t.second.length());
					}
					stringLengths.push_back(this->mTitle.length());

					std::sort(stringLengths.begin(), stringLengths.end(), [](u32 a, u32 b) {
						return a > b;
					});

					return stringLengths.front();
				};
				
				auto toUpper = [this](std::string str)
				{
					std::string tmp;
					for(auto& character : str) {
						tmp += std::toupper(character);
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

				TextColor backup = GetColors();
				Display::SetColors(mForecolor, mBackcolor);
				Display::DrawText({mPos.x + 5, mPos.y + 5}, mTitle);
				Display::SetColors(mBackcolor, mForecolor);
				
				u32 incy = 20;
				for(u16 spot = 0; auto &t : mMenuItems) 
				{
					if(bInFocus && static_cast<u32>(mMenuItemSelection) == spot) 
					{
						if(bEnableHighlight) 
						{
							Display::DrawRect({mPos.x + 2, mPos.y + incy - 2}, dimensions.x - 3, 10, cHighlightColor);
						}

						Display::DrawText({mPos.x + 5, mPos.y + incy}, toUpper(t.second));
					} 
					else 
						Display::DrawText({mPos.x + 5, mPos.y + incy}, t.second);
					incy += mMenuItemSpacing;
					++spot;
				}
						Display::SetColors(backup.Foreground, backup.Background | 0xFF);
			}
		}

		bool MenuItemIsSelected(u32 item) 
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
		void SetItemSpacing(u32 spacing) {
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

	public:
		LibMenu* AddMenu(u32 id, char* text, const LibPos position, const LibColor fg_color, const LibColor bg_color)
		{
			menuList.emplace_back(new LibMenu(id, text, position, fg_color, bg_color));
			return menuList.back();
		}

		bool AllMenusClosed() 
		{
			std::for_each(std::begin(menuList), std::end(menuList), 
			[](auto& menus) {
				if(menus->MenuIsShowing() || menus->bInFocus) {
					return false;
				}
			});
	
			return true;
		}

		void CloseFocusedMenus() 
		{
			std::for_each(std::begin(menuList), std::end(menuList), 
			[](auto& menus) {
				if(menus->bInFocus) {
					menus->Hide();
				}
			});
		}

		void CloseAllMenus() 
		{
			std::for_each(std::begin(menuList), std::end(menuList), 
			[](auto& menus) {
				menus->Hide();
			});
		}

		LibMenu* operator [](u32 id) { return menuList.at(id); }
};

#endif