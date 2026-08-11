# Qt QML QRhi Renderer

We heard you like GUIs, so we put Clay UI in your QML UI so you can use GUI while you using a GUI.

Renders into QSGNode - a way for making custom Items in QML user interface.

Based on the GLES3 renderer.

## ClayFastQSGRenderNode

It's called "Fast" because it doesn't implement clipping and renders all the text as one QSGTextNode.
