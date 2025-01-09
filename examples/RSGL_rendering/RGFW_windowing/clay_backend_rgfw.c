Clay_Vector2 mousePosition;

void clay_RGFW_update(RGFW_window* win, double deltaTime) {

                    
    RGFW_Event ev = win->event;
    switch (ev.type) {
        case RGFW_mouseButtonPressed: {
            switch (ev.button) {
                case RGFW_mouseScrollUp:
                case RGFW_mouseScrollDown:
                    Clay_UpdateScrollContainers(
                        false,
                        (Clay_Vector2) { 0, ev.scroll },
                        0
                    );
                    break;
                default: 
                    Clay_SetPointerState(mousePosition, RGFW_isMousePressed(win, RGFW_mouseLeft));
                    break;
            }
            break;
        }
        case RGFW_mouseButtonReleased:
            Clay_SetPointerState(mousePosition, RGFW_isMousePressed(win, RGFW_mouseLeft));
            break;
        case RGFW_mousePosChanged:
            mousePosition = (Clay_Vector2){ (float)ev.point.x, (float)ev.point.y };
            Clay_SetPointerState(mousePosition, RGFW_isMousePressed(win, RGFW_mouseLeft));
            break;

        case RGFW_windowResized:
            Clay_SetLayoutDimensions((Clay_Dimensions) { (float)win->r.w, (float)win->r.h });
            break;
        default: break;
    }
}