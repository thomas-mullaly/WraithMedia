/****************************************************************************
 *  Copyright (C) 2010 Thomas Mullaly <tmullaly88@gmail.com>                *
 *                                                                          *
 *  This program is free software: you can redistribute it and/or modify    *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, either version 3 of the License, or       *
 *  (at your option) any later version.                                     *
 *                                                                          *
 *  This program is distributed in the hope that it will be useful,         *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *  You should have received a copy of the GNU General Public License       *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

#include <QtGui/QApplication>

#include <wraith/core/Bootstrapper.hpp>
#include <wraith/gui/MainWindow.hpp>

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

using namespace wraith;
using namespace wraith::gui;

int main(int argc, char **argv) {
#ifdef Q_WS_X11
    XInitThreads();
#endif
    QApplication app(argc, argv);
    app.setApplicationName("wraith");

    Bootstrapper bootStrapper;
    bootStrapper.bootServices();

    MainWindow wnd;
    wnd.show();

    return app.exec();
}
