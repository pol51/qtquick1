/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qtest.h>
#include <QtQuick1/qdeclarativecomponent.h>
#include <QtQuick1/qdeclarativeengine.h>
#include <QtQuick1/qdeclarativeitem.h>
#include <QtWidgets/qgraphicsview.h>
#include <QtWidgets/qgraphicsscene.h>
#include <QtWidgets/qapplication.h>
#include <QtGui/qpa/qwindowsysteminterface.h>

class tst_qdeclarativeapplication : public QObject
{
    Q_OBJECT
public:
    tst_qdeclarativeapplication();

private slots:
    void active();
    void layoutDirection();

private:
    QDeclarativeEngine engine;
};

tst_qdeclarativeapplication::tst_qdeclarativeapplication()
{
}

void tst_qdeclarativeapplication::active()
{
    QDeclarativeComponent component(&engine);
    component.setData("import QtQuick 1.0; Item { property bool active: Qt.application.active }", QUrl::fromLocalFile(""));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>(component.create());
    QVERIFY(item);
    QGraphicsScene scene;
    QGraphicsView view(&scene);
    scene.addItem(item);

    // not active
    QVERIFY(!item->property("active").toBool());
    QCOMPARE(item->property("active").toBool(), QApplication::activeWindow() != 0);

    // active
    view.show();
    QApplication::setActiveWindow(&view);
    QVERIFY(QTest::qWaitForWindowActive(&view));
    QCOMPARE(QApplication::activeWindow(), static_cast<QWidget *>(&view));
    QCOMPARE(item->property("active").toBool(), QApplication::activeWindow() != 0);

    // not active again
    QWindowSystemInterface::handleWindowActivated(0);
    QTRY_VERIFY(!item->property("active").toBool());
}

void tst_qdeclarativeapplication::layoutDirection()
{
    QDeclarativeComponent component(&engine);
    component.setData("import QtQuick 1.0; Item { property bool layoutDirection: Qt.application.layoutDirection }", QUrl::fromLocalFile(""));
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem *>(component.create());
    QVERIFY(item);
    QGraphicsScene scene;
    QGraphicsView view(&scene);
    scene.addItem(item);

    // not mirrored
    QCOMPARE(Qt::LayoutDirection(item->property("layoutDirection").toInt()), Qt::LeftToRight);

    // mirrored
    QApplication::setLayoutDirection(Qt::RightToLeft);
    QCOMPARE(Qt::LayoutDirection(item->property("layoutDirection").toInt()), Qt::RightToLeft);

    // not mirrored again
    QApplication::setLayoutDirection(Qt::LeftToRight);
    QCOMPARE(Qt::LayoutDirection(item->property("layoutDirection").toInt()), Qt::LeftToRight);
}

QTEST_MAIN(tst_qdeclarativeapplication)

#include "tst_qdeclarativeapplication.moc"
