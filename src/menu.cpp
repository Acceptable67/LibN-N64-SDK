/*I let ChatGPT autosplit this one from it's source into a header, so it may not be functional -- I haven't tested.*/

#include <libn/menu.hpp>
#include <libn/types.hpp>

LibMenu::LibMenu(const u8 i, const std::string_view t, const LibPos p, const LibColor f, const LibColor b)
    : mId(i), mPos(p), mTitle(t), mForecolor(f), mBackcolor(b)
{
}

void LibMenu::AddMenuItem(const u32 mId, const std::string_view content, const std::function<void()> callback) 
{
    if (mMenuItems.size() < 16) 
    {
        mMenuItems[mId] = content;
        mMenuItemCallbacks.push_back(callback);
        mMenuItemCount++;
    }
}

void LibMenu::AddMenuItem(u32 mId, std::string content) 
{
    if (mMenuItems.size() < 16) 
    {
        mMenuItems[mId] = content;
        mMenuItemCallbacks.push_back([]() {}); // Default empty callback
        mMenuItemCount++;
    }
}

void LibMenu::MoveSelectionUp()   
{ 
    mMenuItemsSelected.fill(false);

    if ((mMenuItemSelection - 1) >= 0 && bInFocus && MenuIsShowing()) 
    {
        mMenuItemSelection -= 1; 
    }
}

void LibMenu::MoveSelectionDown() 
{ 
    mMenuItemsSelected.fill(false);

    if ((mMenuItemSelection + 1) < mMenuItemCount && bInFocus && MenuIsShowing()) 
    {
        mMenuItemSelection += 1; 
    }
}

void LibMenu::Show()
{
    if (bMenuIsShowing) 
    {
        auto findLargestMenuItem = [&]() -> u32
        {
            std::vector<u32> stringLengths;
            for (auto& t : mMenuItems) 
            {
                stringLengths.push_back(t.second.length());
            }
            stringLengths.push_back(mTitle.length());

            std::sort(stringLengths.begin(), stringLengths.end(), std::greater<u32>());
            return stringLengths.front();
        };
        
        auto toUpper = [](std::string str)
        {
            std::transform(str.begin(), str.end(), str.begin(), ::toupper);
            return str;
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
        for (u16 spot = 0; auto &t : mMenuItems) 
        {
            if (bInFocus && static_cast<u32>(mMenuItemSelection) == spot) 
            {
                if (bEnableHighlight) 
                {
                    Display::DrawRect({mPos.x + 2, mPos.y + incy - 2}, dimensions.x - 3, 10, cHighlightColor);
                }

                Display::DrawText({mPos.x + 5, mPos.y + incy}, toUpper(t.second));
            } 
            else 
            {
                Display::DrawText({mPos.x + 5, mPos.y + incy}, t.second);
            }
            incy += mMenuItemSpacing;
            ++spot;
        }
        Display::SetColors(backup.Foreground, backup.Background | 0xFF);
    }
}

bool LibMenu::MenuItemIsSelected(u32 item) 
{
    return mMenuItemsSelected[item];
}

bool LibMenu::MenuIsShowing() 
{
    return bMenuIsShowing;
}

void LibMenu::Hide() 
{
    bMenuIsShowing = false;
    SetUnfocused();
}

void LibMenu::EnableShow() 
{
    bMenuIsShowing = true;
}

void LibMenu::EnableHighlight() 
{
    bEnableHighlight = true;
}

void LibMenu::DisableHighlight() 
{
    bEnableHighlight = false;
}

void LibMenu::SetHighlightColor(LibColor c) 
{
    cHighlightColor = c;
}

void LibMenu::WaitKeyPress() 
{
    if (bInFocus)
    {
        mMenuItemsSelected.at(mMenuItemSelection) = true;
        mMenuItemCallbacks.at(mMenuItemSelection)();
    }
}

void LibMenu::SetItemSpacing(u32 spacing) 
{
    mMenuItemSpacing = spacing;
}

void LibMenu::SetFocused() 
{
    bInFocus = true;
}

void LibMenu::SetUnfocused() 
{
    bInFocus = false;
}

// LibMenuManager Methods
LibMenu* LibMenuManager::AddMenu(u32 id, std::string_view text, const LibPos position, const LibColor fg_color, const LibColor bg_color)
{
    menuList.emplace_back(new LibMenu(id, text, position, fg_color, bg_color));
    return menuList.back();
}

bool LibMenuManager::AllMenusClosed() 
{
    return std::all_of(menuList.begin(), menuList.end(), 
    [](const auto& menus) {
        return !menus->MenuIsShowing() && !menus->bInFocus;
    });
}

void LibMenuManager::CloseFocusedMenus() 
{
    std::for_each(menuList.begin(), menuList.end(), 
    [](auto& menus) {
        if (menus->bInFocus) {
            menus->Hide();
        }
    });
}

void LibMenuManager::CloseAllMenus() 
{
    std::for_each(menuList.begin(), menuList.end(), 
    [](auto& menus) {
        menus->Hide();
    });
}

LibMenu* LibMenuManager::operator[](u32 id) 
{ 
    return menuList.at(id); 
}
