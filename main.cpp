/*
    Copyright © 2021 Toksisitee <meypcm@gmail.com>
    PopSoundEditor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    PopSpriteEditor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "Common.h"


#include "mainwindow.h"

#include "Editor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName(EDITOR_NAME);
    a.setOrganizationDomain("https://github.com/Toksisitee");
    a.setOrganizationName("Toksisitee");
    a.setApplicationName(EDITOR_NAME);
    a.setApplicationVersion(EDITOR_VERSION);
    MainWindow w;
    w.show();
    return a.exec();
}
