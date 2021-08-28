#ifndef EDITOR_H
#define EDITOR_H

#include <QApplication>
#include "Version.h"

#define	EDITOR_DATE     __DATE__ " " __TIME__

#ifdef QT_DEBUG
#define EDITOR_BUILD    "(DEBUG)"
#else
#define EDITOR_BUILD    "(RELEASE)"
#endif

#endif // EDITOR_H
