//!
//! @file   LibraryWidget.h
//! @author Björn Eriksson <bjorn.eriksson@liu.se>
//! @date   2010-02-05
//!
//! @brief Contains classes for Library Widgets
//!
//$Id$

#ifndef LIBRARYWIDGET_H
#define LIBRARYWIDGET_H

#include "common.h"
#include "HopsanCore.h"

#include <QListWidget>
#include <QStringList>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidgetItem>
#include <QStringList>

class AppearanceData;
class LibraryContentItem;

class LibraryContentItem : public QListWidgetItem
{
public:
    LibraryContentItem(AppearanceData *pAppearanceData, QListWidget *parent = 0);
    LibraryContentItem(const QListWidgetItem &other);
    AppearanceData *getAppearanceData();
    QString getTypeName();

//public slots:
    void selectIcon(graphicsType gfxType=USERGRAPHICS);

private:
    AppearanceData *mpAppearanceData;

};

//Forward declaration
class LibraryWidget;

class LibraryContent : public QListWidget
{
    Q_OBJECT

public:
    LibraryContent(LibraryContent *pParentLibraryContent, QString mapKey, LibraryWidget *pParentLibraryWidget, QTreeWidgetItem *pParentTreeWidgetItem);
    bool mIsUserLib;
    QTreeWidgetItem *mpParentTreeWidgetItem;
    QString mMapKey;

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

public slots:
    virtual void highLightItem(QListWidgetItem *item);

private:
    QPoint dragStartPosition;
    LibraryWidget *mpParentLibraryWidget;
    QListWidgetItem *mpHoveredItem;
};


class MainWindow;

class LibraryWidget : public QWidget
{
    Q_OBJECT

    friend class LibraryContent;
    friend class LibraryTreeWidget;

public:
    //Member functions
    LibraryWidget(MainWindow *parent = 0);
    void addEmptyLibrary(QString libraryName, QString parentLibraryName=QString());
    void addLibrary(QString libDir, QString parentLib=QString());
    void addLibraryContentItem(QString libraryName, QString parentLibraryName, LibraryContentItem *newComponent);
    void addExternalLibrary(QString libDir);
    AppearanceData *getAppearanceData(QString componentType);
    AppearanceData *getAppearanceDataByDisplayName(QString displayName);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);

public slots:
    void addLibrary();
    void setGfxType(graphicsType gfxType);

private slots:
    void showLib(QTreeWidgetItem * item, int column);
    void hideAllLib();

private:
    //Member variables
    MainWindow *mpParentMainWindow;

    QHash<QString, LibraryContent*> mLibraryContentPtrsMap;
    QMultiMap<QString, LibraryContentItem*> mLibraryContentItemPtrsMap;

    QHash<QString, QString> mName2TypeMap; //!< @todo This is a temporary hack

    QLabel *mpComponentNameField;

    QTreeWidget *mpTree;
    QVBoxLayout *mpGrid;
};


class LibraryTreeWidget : public QTreeWidget
{
public:
    LibraryTreeWidget(LibraryWidget *parent = 0);
    LibraryWidget *mpParentLibraryWidget;

protected:
    virtual void contextMenuEvent(QContextMenuEvent *);
};

#endif // LIBRARYWIDGET_H
