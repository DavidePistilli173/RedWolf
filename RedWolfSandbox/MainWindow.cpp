#include "MainWindow.hpp"

MainWindow::MainWindow() : BaseWindow(window_title, window_width, window_height, false) {}

void MainWindow::userHandle_(const rw::events::BaseEvent&, const rw::core::BaseObject*) {}