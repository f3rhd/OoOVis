#include <Visualizer/Units/DrawElement.h>
#include <Core/Commit/ReorderBufferEntry.h>
namespace OoOVisual
{
    namespace Visualizer
    {
        
        struct Draw_Element_Reorder_Buffer : Draw_Element {
            Draw_Element_Reorder_Buffer(
                DRAW_ELEMENT_ID id,
                ImVec2 position,
                ImVec2 dimension
            ) : Draw_Element(id, position, dimension) {}
            void show_tooltip() override;
            void show_detailed() override;
        private:
            void render_entry_details(Core::Reorder_Buffer_Entry* entry);
        };
    } // namespace Visualizer
    
    
} // namespace OoOVisual
