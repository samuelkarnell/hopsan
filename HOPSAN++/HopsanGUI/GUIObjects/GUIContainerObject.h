//$Id$

#ifndef GUICONTAINEROBJECT_H
#define GUICONTAINEROBJECT_H

#include "GUIModelObject.h"
#include "../CoreAccess.h"
#include "GUIWidgets.h"
//#include "../UndoStack.h"

//Forward Declarations
class ProjectTab;
class UndoStack;
class MainWindow;
class QGraphicsScene;

class GUIContainerObject : public GUIModelObject
{
    Q_OBJECT
public:
    enum CONTAINERSTATUS {CLOSED, OPEN, ROOT};
    GUIContainerObject(QPoint position, qreal rotation, const GUIModelObjectAppearance* pAppearanceData, selectionStatus startSelected = DESELECTED, graphicsType gfxType = USERGRAPHICS, GUIContainerObject *system=0, QGraphicsItem *parent = 0);
    void makeRootSystem();

    //Scene and Core access
    QGraphicsScene *getContainedScenePtr();
    virtual CoreSystemAccess *getCoreSystemAccessPtr();

    //Handle GuiModelObjects and GuiWidgets
    void addTextWidget(QPoint position, undoStatus undoSettings=UNDO);
    void addBoxWidget(QPoint position, undoStatus undoSettings=UNDO);
    GUIModelObject *addGUIModelObject(GUIModelObjectAppearance* pAppearanceData, QPoint position, qreal rotation=0, selectionStatus startSelected = DESELECTED, nameVisibility nameStatus = NAMEVISIBLE, undoStatus undoSettings = UNDO);
    GUIModelObject *getGUIModelObject(QString name);
    void deleteGUIModelObject(QString componentName, undoStatus undoSettings=UNDO);
    void renameGUIModelObject(QString oldName, QString newName, undoStatus undoSettings=UNDO);
    bool hasGUIModelObject(QString name);

    //Handle connectors
    GUIConnector *findConnector(QString startComp, QString startPort, QString endComp, QString endPort);
    bool hasConnector(QString startComp, QString startPort, QString endComp, QString endPort);
    void removeConnector(GUIConnector* pConnector, undoStatus undoSettings=UNDO);
    void setIsCreatingConnector(bool isCreatingConnector);
    bool getIsCreatingConnector();

    //Handle container appearance
    QString getIsoIconPath();
    QString getUserIconPath();
    void setIsoIconPath(QString path);
    void setUserIconPath(QString path);
    void refreshExternalPortsAppearanceAndPosition();
    void calcSubsystemPortPosition(const double w, const double h, const double angle, double &x, double &y); //!< @todo maybe not public

    bool isObjectSelected();
    bool isConnectorSelected();

    //This (overloaded versions) are used in containerPropertiesDialog by systems
    virtual void setTypeCQS(QString /*typestring*/){assert(false);}
    virtual size_t getNumberOfLogSamples(){assert(false);}
    virtual void setNumberOfLogSamples(size_t /*nSamples*/){assert(false);}
    virtual void removeFavoriteParameterByComponentName(QString /*componentName*/){assert(false);}

    //Public member variable
    //!< @todo make this private later
    QFileInfo mModelFileInfo; //!< @todo should not be public
    UndoStack *mUndoStack;
    ProjectTab *mpParentProjectTab;

    QList<GUIConnector *> mSelectedSubConnectorsList;
    QList<GUIConnector *> mSubConnectorList;

    //SHOULD BE PROTECTED
    typedef QHash<QString, GUIModelObject*> GUIModelObjectMapT;
    GUIModelObjectMapT mGUIModelObjectMap;
    QList<GUITextWidget *> mTextWidgetList;
    QList<GUIBoxWidget *> mBoxWidgetList;
    QList<GUIModelObject *> mSelectedGUIObjectsList;
    QList<GUIWidget *> mSelectedGUIWidgetsList;

    bool mPortsHidden;
    bool mUndoDisabled;
    bool mIsRenamingObject;
    bool mJustStoppedCreatingConnector;

    GUIModelObject *mpTempGUIModelObject;
    GUIConnector *mpTempConnector;
    graphicsType mGfxType;

    size_t mHighestWidgetIndex;
    QMap<size_t, GUIWidget *> mWidgetMap;

public slots:
        //Selection
    void selectAll();
    void deselectAll();
    void deselectSelectedNameText();
        //show/hide
    void hideNames();
    void showNames();
    void hidePorts(bool doIt);
        //Create and remove
    void createConnector(GUIPort *pPort, undoStatus undoSettings=UNDO);
    void groupSelected(QPointF pt);
        //CopyPaste
    void cutSelected(CopyStack *xmlStack = 0);
    void copySelected(CopyStack *xmlStack = 0);
    void paste(CopyStack *xmlStack = 0);
        //UndoRedo
    void undo();
    void redo();
    void clearUndo();
    void disableUndo();
    void updateUndoStatus();
        //Appearance and settings
    void setGfxType(graphicsType gfxType);
    void openPropertiesDialogSlot();
        //Enter and exit a container object
    //! @todo dont know if these names are good
    void enterContainer();
    void exitContainer();

signals:
        //Selection
    void deselectAllNameText();
    void deselectAllGUIObjects();
    void selectAllGUIObjects();
    void deselectAllGUIConnectors();
    void selectAllGUIConnectors();
        //HideShow
    void hideAllNameText();
    void showAllNameText();
        //Other
    void checkMessages();
    void deleteSelected();
    void setAllGfxType(graphicsType);
    void componentChanged();


protected:
        //Protected methods
    virtual void createPorts();
    virtual void createExternalPort(QString portName);
    virtual void openPropertiesDialog();
        //Protected overloaded Qt methods
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

protected slots:


private:
    bool mIsCreatingConnector;

    CONTAINERSTATUS getContainerStatus();
    CONTAINERSTATUS mContainerStatus;

    QGraphicsScene *mpScene;

    double mPasteOffset;

};

#endif // GUICONTAINEROBJECT_H
