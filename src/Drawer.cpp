///
/// @file Drawer.cpp
/// @author Alix ANNERAUD (alix.anneraud@outlook.fr)
/// @brief
/// @version 0.1.0
/// @date 12-09-2022
///
/// @copyright Copyright (c) 2022
///

#include "Shell.hpp"

Shell_Class::Drawer_Class::Drawer_Class(Shell_Class *Shell_Pointer) : Shell_Pointer(Shell_Pointer)
{
}

void Shell_Class::Drawer_Class::Set_Interface()
{
    using namespace Graphics_Types;

    Window.Create(Shell_Pointer);

    Close();
    Window.Set_Minimize_Button_Hidden(true);
    Window.Set_Title("Drawer");
    Window.Get_Close_Button().Remove_All_Events();
    Window.Get_Close_Button().Add_Event(Shell_Pointer, Graphics_Types::Event_Code_Type::Clicked);

    // - Set flex container
    Window.Get_Body().Set_Flex_Flow(Flex_Flow_Type::Column_Wrap);
    Window.Get_Body().Set_Style_Pad_Row(5, 0);
    Window.Get_Body().Set_Style_Pad_Column(5, 0);

    {
        Object_Type Container, Icon_Container;
        Graphics_Types::Label_Type Icon_Label, Label;
        Static_String_Class<Default_Software_Name_Length> Name;

        uint8_t Handle_Count = Softwares.Get_Handle_Count();

        for (Byte_Type i = 0; i < Handle_Count; i++)
        {
            if (Shell_Pointer->Get_Handle() == Softwares.Get_Handle(i))
                continue;

            Container.Create(Window.Get_Body());
            Container.Set_Size(12 * 8, 11 * 8);
            Container.Set_Style_Background_Opacity(Opacity_Type::Transparent, 0);
            Container.Set_Style_Pad_All(0, 0);
            Container.Add_Event(Shell_Pointer, Graphics_Types::Event_Code_Type::Clicked);
            Container.Set_Flex_Flow(Flex_Flow_Type::Column);
            Container.Set_Flex_Alignment(Flex_Alignment_Type::Space_Evenly, Flex_Alignment_Type::Center, Flex_Alignment_Type::Center);

            Softwares.Get_Handle(i)->Get_Name(Name);

            // - Set software icon (color according to it's name)
            Icon_Container.Create(Container);
            //   Icon_Container.Set_Alignment(Alignment_Type::Top_Middle);
            Icon_Container.Add_Flag(Flag_Type::Event_Bubble);
            Icon_Label.Create(Icon_Container);
            Shell_Pointer->Get_Software_Icon(Icon_Container, Name);

            // - Set software label
            Label.Create(Container, Name, false, Percentage(100));
            //   Label.Set_Alignment(Alignment_Type::Bottom_Middle);
            Label.Set_Long_Mode(Graphics_Types::Long_Type::Dot);
            Label.Add_Flag(Flag_Type::Event_Bubble);
            Label.Set_Style_Text_Alignment(Text_Alignment_Type::Center, 0);

            Icon_Container.Clear_Pointer();
            Icon_Label.Clear_Pointer();
            Container.Clear_Pointer();
            Label.Clear_Pointer();
        }
    }
}

Shell_Class::Drawer_Class::~Drawer_Class()
{
    Window.Delete();
}

void Shell_Class::Drawer_Class::Execute_Instruction(const Instruction_Type &Instruction)
{
    if (Instruction.Get_Sender() == &Graphics)
    {
        using namespace Graphics_Types;
        Object_Type Current_Target = Instruction.Graphics.Get_Current_Target(); // Get icon container
        switch (Instruction.Graphics.Get_Code())
        {
        case Graphics_Types::Event_Code_Type::Clicked:
        {
            if (Current_Target == Window.Get_Close_Button())
            {
                Close();
            }
            else if (Current_Target.Get_Parent() == Window.Get_Body())
            {
                Size_Type Child_Index = Window.Get_Body().Get_Child_Index(Current_Target); // Get icon container index
                uint8_t Handle_Count = Softwares.Get_Handle_Count();
                for (uint8_t j = 0; j < Softwares.Get_Handle_Count(); j++)
                {
                    if (Shell_Pointer->Get_Handle() == Softwares.Get_Handle(j))
                    {
                        j++;
                        Child_Index++;
                    }

                    if (Child_Index == j)
                    {
                        Softwares.Get_Handle(j)->Create_Instance(Shell_Pointer->Get_Owner_User());
                        Shell_Pointer->Desk.Refresh();
                        Close();
                        break;
                    }
                }
            }
        }
        break;
        default:
            break;
        }
    }
}

void Shell_Class::Drawer_Class::Open()
{
    using namespace Graphics_Types;

    Window.Set_State(Window_State_Type::Maximized);
}

void Shell_Class::Drawer_Class::Close()
{
    Window.Set_State(Graphics_Types::Window_State_Type::Minimized);
}

bool Shell_Class::Drawer_Class::Is_Openned()
{
    using namespace Graphics_Types;

    if (!Window)
        return false;

    return Window.Get_State() == Window_State_Type::Maximized;
}