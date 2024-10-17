#ifndef LIBN_MENU_H
#define LIBN_MENU_H

#include <libn/types.hpp>
#include <functional>
#include <map>
#include <vector>
#include <string>
#include <algorithm> // For std::sort

class LibMenu 
{
private:
    u8 mId;
    LibPos mPos;
    std::string mTitle;
    LibColor mForecolor, mBackcolor;

    u32 mMenuItemSpacing = 10;
    u32 mMenuItemCount = 0;
    f32 mMenuItemSelection = 0.0f;

    std::map<u32, std::string> mMenuItems;
    std::vector<std::function<void()>> mMenuItemCallbacks;
    std::array<bool, 16> mMenuItemsSelected{false};

    bool bMenuIsShowing = true;
    bool bEnableHighlight = true;
    bool bInFocus = false;
    LibColor cHighlightColor = LibColor::RED;

public:
    LibMenu(const u8 i, const std::string_view t, const LibPos p, const LibColor f = BLACK, const LibColor b = WHITE);
    LibMenu() = default;
    ~LibMenu() = delete;

    void AddMenuItem(const u32 mId, const std::string_view content, const std::function<void()> callback);
    void AddMenuItem(u32 mId, std::string content);
    void MoveSelectionUp();
    void MoveSelectionDown();
    void Show();
    bool MenuItemIsSelected(u32 item);
    bool MenuIsShowing();
    void Hide();
    void EnableShow();
    void EnableHighlight();
    void DisableHighlight();
    void SetHighlightColor(LibColor c);
    void WaitKeyPress();
    void SetItemSpacing(u32 spacing);
    void SetFocused();
    void SetUnfocused();
};

class LibMenuManager
{
private:
    std::vector<LibMenu*> menuList;

public:
    LibMenu* AddMenu(u32 id, std::string_view text, const LibPos position, const LibColor fg_color, const LibColor bg_color);
    bool AllMenusClosed();
    void CloseFocusedMenus();
    void CloseAllMenus();
    LibMenu* operator [](u32 id);
};

#endif // LIBN_MENU_H
