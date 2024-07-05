#ifndef __STYLE_H_
#define __STYLE_H_
#include <stdint.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>


struct style_pair_s {
  const char *attr;
  uint32_t val;
};

struct style_table_s {
  const char *type;
  struct style_pair_s table[10]; // Should adjust if more props !
};

struct style_val_s {
  bool present;
  const char *type;
  uint32_t val;
  uint32_t imgui_val;
};

int style_get_cnt(struct style_table_s *table, const char *type);
void free_style(struct style_val_s **s);
struct style_val_s **create_style(struct style_table_s *table, char *type);
struct style_val_s **add_style(struct style_val_s **target, struct style_table_s *table, char *type);
struct style_pair_s  *style_get_idx(struct style_table_s *table, const char *type, uint32_t idx);



static struct style_table_s color_table[] = 
  {
    {"text", {
	{"color", ImGuiCol_Text},
	{"color_disabled", ImGuiCol_TextDisabled},
	{"color_selected_bg", ImGuiCol_TextSelectedBg}, 
	{NULL, 0}
      },
    },

    {"window", {
	 {"color_bg", ImGuiCol_WindowBg},
	 {NULL, 0}
      },
    },

    {"child", {
	{"color_bg", ImGuiCol_ChildBg},
	{NULL, 0}
      }
    },

    {"popup", {
	{"color_bg", ImGuiCol_PopupBg},
	{NULL, 0}
      }
    },
    {"border", {
	{"color", ImGuiCol_Border}, 
	{"color_shadow", ImGuiCol_BorderShadow},
	{NULL, 0}
      },
    },
    {"frame", {
	{"color_bg", ImGuiCol_FrameBg}, 
	{"color_bg_hovered", ImGuiCol_FrameBgHovered}, 
	{"color_bg_active", ImGuiCol_FrameBgActive},
	{NULL, 0}
      },
    },
    {"title",{
	{"color_bg", ImGuiCol_TitleBg}, 
	{"color_bg_active", ImGuiCol_TitleBgActive}, 
	{"color_bg_collapsed", ImGuiCol_TitleBgCollapsed},
	{NULL, 0}
      }
    },
    {"menubar", {
     	{"color_bar_bg", ImGuiCol_MenuBarBg},
	{NULL, 0}
      },
    },
    {"scrollbar",{
	{"color_bg", ImGuiCol_ScrollbarBg}, 
	{"color_grab", ImGuiCol_ScrollbarGrab}, 
	{"color_grab_hovered", ImGuiCol_ScrollbarGrabHovered}, 
	{"color_grab_active", ImGuiCol_ScrollbarGrabActive},
	{NULL, 0}
      }
    },
    {"check", {
	{"color_mark", ImGuiCol_CheckMark},
	{NULL, 0}
      },
    },
    {"slider", {
	{"color_grab", ImGuiCol_SliderGrab}, 
	{"color_grab_active", ImGuiCol_SliderGrabActive},
	{NULL, 0}
      },
    },
    {"button", {
	{"color", ImGuiCol_Button}, 
	{"color_hovered", ImGuiCol_ButtonHovered}, 
	{"color_active", ImGuiCol_ButtonActive},
	{NULL, 0}
      }
    },
    {"header", {
        {"color", ImGuiCol_Header}, 
	{"color_hovered", ImGuiCol_HeaderHovered}, 
	{"color_active", ImGuiCol_HeaderActive},
	{NULL, 0}
      }
    },
    {"separator", {
	{"color", ImGuiCol_Separator}, 
	{"color_hovered", ImGuiCol_SeparatorHovered}, 
	{"color_active", ImGuiCol_SeparatorActive},
	{NULL, 0}
      }
    },
    {"resizegrip", {
	{"color", ImGuiCol_ResizeGrip}, 
	{"color_hovered", ImGuiCol_ResizeGripHovered}, 
	{"color_active", ImGuiCol_ResizeGripActive},
	{NULL, 0}
      }
    },

    {"tab", {
	{"color", ImGuiCol_Tab}, 
	{"color_selected", ImGuiCol_TabSelected}, 
	{"color_selected_overline", ImGuiCol_TabSelectedOverline}, 
	{"color_dimmed", ImGuiCol_TabDimmed}, 
	{"color_dimmed_selected", ImGuiCol_TabDimmedSelected},
	{"color_dimmed_selected_overline", ImGuiCol_TabDimmedSelectedOverline},
	{NULL, 0}
      }
    },
    {"docking", {
	{"color_preview", ImGuiCol_DockingPreview}, 
	{"color_empty_bg", ImGuiCol_DockingEmptyBg},
	{NULL, 0}
      },
    },
    {"plotline", {
	{"color_lines", ImGuiCol_PlotLines}, 
	{"color_lines_hovered", ImGuiCol_PlotLinesHovered}, 
	{"color_histogram", ImGuiCol_PlotHistogram}, 
	{"color_histogram_hovered", ImGuiCol_PlotHistogramHovered},
	{NULL, 0}
      }
    },
    {"table", {
	{"color_header_bg", ImGuiCol_TableHeaderBg}, 
	{"color_border_strong", ImGuiCol_TableBorderStrong}, 
	{"color_border_light", ImGuiCol_TableBorderLight}, 
	{"color_row_bg", ImGuiCol_TableRowBg}, 
	{"color_row_bg_alt", ImGuiCol_TableRowBgAlt},
	{NULL, 0}
      }
    },
    {"dragdrop", {
	{"color_target", ImGuiCol_DragDropTarget},
	{NULL, 0}
      }
    },
    {"nav", {
	{"color_highlight", ImGuiCol_NavHighlight}, 
	{"color_windowing_highlight", ImGuiCol_NavWindowingHighlight}, 
	{"color_windowing_dim_bg", ImGuiCol_NavWindowingDimBg},
	{NULL, 0}
      }
    },
    {"modal", {
	{"color_window_dim_bg", ImGuiCol_ModalWindowDimBg},
	{NULL, 0}
      }
    },
    {NULL, NULL}
  };
  
#endif
