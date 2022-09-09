#include "MainWindow.hpp"

MainWindow::MainWindow(rw::RedWolfManager& rw) : BaseWindow(rw, window_title, window_width, window_height, false) {}

void MainWindow::userHandle_(const rw::events::BaseEvent&, const rw::core::BaseObject*) {}