#include "PythonPlugins.h"
#include "Classes/Qt/Main/mainwindow.h"
#include "Classes/Qt/Scene/SceneSignals.h"
#include "Classes/Qt/TextureBrowser/TextureBrowser.h"
#include "Classes/Qt/TextureBrowser/TextureCache.h"
#include "Classes/Qt/Tools/ExportSceneDialog/ExportSceneDialog.h"
#include "Classes/SceneManager/Private/SceneRenderWidget.h"
#include "Classes/SceneManager/Private/SignalsAccumulator.h"

#include <REPlatform/Commands/Private/RECommandStack.h>
#include <REPlatform/Commands/TilemaskEditorCommands.h>
#include <REPlatform/DataNodes/ProjectManagerData.h>
#include <REPlatform/DataNodes/SceneData.h>
#include <REPlatform/DataNodes/Settings/GlobalSceneSettings.h>
#include <REPlatform/DataNodes/Settings/RESettings.h>
#include <REPlatform/DataNodes/SpritesPackerModule.h>
#include <REPlatform/Deprecated/EditorConfig.h>
#include <REPlatform/Global/Constants.h>
#include <REPlatform/Global/GlobalOperations.h>
#include <REPlatform/Scene/SceneEditor2.h>
#include <REPlatform/Scene/SceneHelper.h>
#include <REPlatform/Scene/Systems/CameraSystem.h>
#include <REPlatform/Scene/Systems/CollisionSystem.h>
#include <REPlatform/Scene/Systems/EditorParticlesSystem.h>
#include <REPlatform/Scene/Systems/EditorVegetationSystem.h>
#include <REPlatform/Scene/Systems/LandscapeEditorDrawSystem.h>
#include <REPlatform/Scene/Systems/SelectionSystem.h>
#include <REPlatform/Scene/Systems/StructureSystem.h>
#include <REPlatform/Scene/Utils/SceneSaver.h>

#include <TArc/WindowSubSystem/ActionUtils.h>
#include <TArc/WindowSubSystem/QtAction.h>
#include <TArc/WindowSubSystem/UI.h>

#include <QtTools/FileDialogs/FindFileDialog.h>
#include <QtTools/ProjectInformation/FileSystemCache.h>

#include <Base/Any.h>
#include <Base/FastName.h>
#include <Base/GlobalEnum.h>
#include <Base/String.h>
#include <Engine/EngineContext.h>
#include <FileSystem/FileSystem.h>
#include <Functional/Function.h>
#include <Reflection/ReflectedObject.h>
#include <Reflection/ReflectedType.h>
#include <Reflection/Reflection.h>
#include <Render/DynamicBufferAllocator.h>
#include <Render/Renderer.h>

#include <QActionGroup>
#include <QList>
#include <QMimeData>
#include <QProcess>
#include <QString>
#include <QUrl>
#include <QtConcurrent/QtConcurrent>
#include <QtGlobal>

#include <fstream>
#include <iostream>


PythonPlugins::PythonPlugins()
{
    using namespace DAVA;
}

PythonPlugins::~PythonPlugins() = default;

void PythonPlugins::OnContextCreated(DAVA::DataContext* context)
{

}

void PythonPlugins::OnContextDeleted(DAVA::DataContext* context)
{

}

void PythonPlugins::OnContextWillBeChanged(DAVA::DataContext* current, DAVA::DataContext* newOne)
{
    using namespace DAVA;
    if (current == nullptr)
    {
        return;
    }
}

void PythonPlugins::OnContextWasChanged(DAVA::DataContext* current, DAVA::DataContext* oldOne)
{
    using namespace DAVA;
    if (current == nullptr)
    {
        return;
    }
}

void PythonPlugins::OnWindowClosed(const DAVA::WindowKey& key)
{
}

void PythonPlugins::PostInit()
{
    using namespace DAVA;

    DAVA::UI* ui = GetUI();

    CreateModuleControls(ui);
    CreateModuleActions(ui);
}

void PythonPlugins::RunPluginAsync(DAVA::String pluginName, DAVA::FilePath scriptPath)
{
    QtConcurrent::run([=]()
                      { RunPlugin(pluginName, scriptPath); });
}

void PythonPlugins::RunPlugin(DAVA::String pluginName, DAVA::FilePath scriptPath)
{
    using namespace DAVA;

    Logger::Warning("[PythonPlugins::RunPlugin] %s", pluginName.c_str());

    QProcess pythonProcess;

    QString scriptDir = QString::fromStdString(scriptPath.GetDirectory().GetAbsolutePathname());
    QString scriptFile = QString::fromStdString(scriptPath.GetAbsolutePathname());

    pythonProcess.setWorkingDirectory(scriptDir);

    QStringList arguments;
    arguments << scriptFile;

    pythonProcess.start("python", arguments);

    if (!pythonProcess.waitForStarted())
    {
        Logger::Error("Failed to start python process.");
        return;
    }

    if (!pythonProcess.waitForFinished())
    {
        Logger::Error("Python script did not finish properly.");
        return;
    }

    QString output = pythonProcess.readAllStandardOutput();
    QString errorOutput = pythonProcess.readAllStandardError();

    for (const QString& line : output.split('\n'))
    {
        if (!line.trimmed().isEmpty())
        {
            Logger::Info("[Python output] %s", line.toStdString().c_str());
        }
    }

    for (const QString& line : errorOutput.split('\n'))
    {
        if (!line.trimmed().isEmpty())
        {
            Logger::Error("[Python error] %s", line.toStdString().c_str());
        }
    }
}

void PythonPlugins::CreateModuleControls(DAVA::UI* ui)
{

}

void PythonPlugins::CreateModuleActions(DAVA::UI* ui)
{
    using namespace DAVA;

    ContextAccessor* accessor = GetAccessor();

    // Python Plugins
    FilePath pluginsPath = FilePath("~res:/ResourceEditor/Plugins/");
    if (!pluginsPath.Exists())
    {
        Logger::Info("[QtMainWindow::SetupPlugins] plugins directory doesn't exists at %s", pluginsPath.GetAbsolutePathname().c_str());
    }
    else
    {
        Vector<FilePath> pluginNames = GetEngineContext()->fileSystem->EnumerateDirectoriesInDirectory(pluginsPath, false);

        for (uint32 pluginIndex = 0; pluginIndex < pluginNames.size(); pluginIndex++)
        {
            FilePath scriptPath(pluginNames[pluginIndex]);
            scriptPath.ReplaceFilename("Main.py");
            if (scriptPath.Exists())
            {
                String pluginName = pluginNames[pluginIndex].GetLastDirectoryName();

                QtAction* action = new QtAction(accessor, QString(pluginName.c_str()));
                connections.AddConnection(action, &QAction::triggered, DAVA::Bind(static_cast<void (PythonPlugins::*)(DAVA::String, DAVA::FilePath)>(&PythonPlugins::RunPluginAsync), this, pluginName, scriptPath));

                ActionPlacementInfo placementInfo;
                placementInfo.AddPlacementPoint(CreateMenuPoint(MenuItems::menuPlugins, { InsertionParams::eInsertionMethod::AfterItem, "Help" }));
                placementInfo.AddPlacementPoint(CreateToolbarPoint("mainToolBar", { InsertionParams::eInsertionMethod::AfterItem, "Plugins" }));

                ui->AddAction(mainWindowKey, placementInfo, action);
            }
        }
    }
}