#include "MainWindow.hpp"

MainWindow::MainWindow(rw::RedWolfManager& rw) : BaseWindow(rw, window_title, window_width, window_height, true)
{
   testEntity_ = std::make_unique<TestEntity>(rw, renderer_);
}

void MainWindow::userHandle_(const rw::events::BaseEvent&, const rw::core::BaseObject*) {}