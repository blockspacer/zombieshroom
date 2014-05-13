#include <RootTools/Treenity/Include/AssetManagerWidget.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootTools/Treenity/Include/Utils.h>
#include <QDesktopServices>
extern RootEngine::GameSharedContext g_engineContext;

AssetManagerWidget::AssetManagerWidget( QWidget* p_parent /*= 0*/ )
	: QWidget(p_parent)
{
	ui.setupUi(this);

	ui.listView_fileBrowser->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(ui.lineEdit_assetSearch,			SIGNAL(textEdited(const QString &)),			this,				SLOT(SearchLineChanged(const QString&)));
	connect(ui.treeView_assetFileBrowser,		SIGNAL(doubleClicked(const QModelIndex&)),		this,				SLOT(FolderSelected(const QModelIndex&)));
	connect(ui.listView_fileBrowser,			SIGNAL(doubleClicked(const QModelIndex&)),		this,				SLOT(FileSelected(const QModelIndex&)));
	connect(ui.listView_fileBrowser,			SIGNAL(clicked(const QModelIndex&)),			this,				SLOT(FileClicked(const QModelIndex&)));
	connect(ui.listView_fileBrowser,			SIGNAL(customContextMenuRequested(const QPoint &)),	this,			SLOT(TreeListContextMenu(const QPoint &)));
	connect(ui.pushButton_back,					SIGNAL(clicked()),								this,				SLOT(NavigateBack()));
	connect(ui.pushButton_collapseall,			SIGNAL(clicked()),								this,				SLOT(CollapseAll()));
	connect(ui.pushButton_expandall,			SIGNAL(clicked()),								this,				SLOT(ExpandAll()));
	connect(ui.horizontalSlider_icon,			SIGNAL(valueChanged(int)),						this,				SLOT(IconSizeChanged(int)));


	//Create folder model for tree view
	m_assetFolderModel = new QFileSystemModel();
	m_assetFolderModel->setRootPath(QString::fromStdString(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets/"));
	m_assetFolderModel->setFilter(QDir::Dirs|QDir::Drives|QDir::NoDotAndDotDot|QDir::AllDirs);
	
	connect(m_assetFolderModel,					SIGNAL(directoryLoaded ( const QString & )),	this,				SLOT(FolderLoaded(const QString&)));

	//Set up tree view and add folder file model
	ui.treeView_assetFileBrowser->setModel(m_assetFolderModel);
	ui.treeView_assetFileBrowser->setColumnWidth(0, 150);
	ui.treeView_assetFileBrowser->hideColumn(1);
	ui.treeView_assetFileBrowser->hideColumn(2);
	ui.treeView_assetFileBrowser->hideColumn(3);
	ui.treeView_assetFileBrowser->setRootIndex(m_assetFolderModel->index(QString::fromStdString(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets/")));


	//Set up list widget
	m_assetFileModel = new QFileSystemModel();
	m_assetFileModel->setRootPath(QString::fromStdString(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets/"));
	//m_assetFileModel->setFilter(QDir::Dirs|QDir::Drives|QDir::NoDotAndDotDot|QDir::AllDirs);

	ui.listView_fileBrowser->setModel(m_assetFileModel);
	ui.listView_fileBrowser->setRootIndex(m_assetFileModel->index(QString::fromStdString(g_engineContext.m_resourceManager->GetWorkingDirectory() + "Assets/")));
	ui.listView_fileBrowser->setDragDropMode(QAbstractItemView::DragOnly);

	//Set up context menues
	m_fileContextMenu = new QMenu("File context", this);
	m_fileContextMenu->addAction(new QAction("Open externally", this));

	m_offsideContextMenu = new QMenu("Offset menu", this);
	m_offsideContextMenu->addAction(new QAction("Create folder", this));

	IconSizeChanged(ui.listView_fileBrowser->iconSize().height());

	m_ddsHandler = new QDDSHandler();
}

AssetManagerWidget::~AssetManagerWidget()
{
	//Clean up
	delete m_assetFileModel;
	delete m_assetFolderModel;
}

/*		Blue print of Asset browser
________________________________________________________
|				|_______________Search bar______________|
|				|										|
|				|										|
|				|										|
|   Tree view	|			List view					|
|				|										|
|				|										|
|               |										|
|               |										|
________________________________________________________

*/
//When typing in the search box
void AssetManagerWidget::SearchLineChanged( const QString& p_val )
{
	if(p_val == "")
	{	//show all files and dirs if search bar is empty
		m_assetFileModel->setFilter(QDir::Files | QDir::Dirs | QDir::Drives | QDir::NoDotAndDotDot | QDir::AllDirs);
	}
	else
	{
		//show only files when filtering files
		m_assetFileModel->setFilter(QDir::Files);
	}
	
	//Set new filter when editing search bar. The filter will look for the input string in the file names and filter on that
	QStringList tempList;
	if(m_currentFilter.isEmpty())
	{
		QString searchValue = "*" + p_val + "*.*";
		tempList << searchValue;
	}
	else
	{
		for (QString filterSuffix : m_currentFilter)
		{
			tempList << "*" + p_val + "*" + filterSuffix;
		}
	}

	m_assetFileModel->setNameFilters(tempList);
	m_assetFileModel->setNameFilterDisables(false);
}

//Double click in tree view
void AssetManagerWidget::FolderSelected( const QModelIndex& p_val )
{
	QFileInfo fileInfo = m_assetFolderModel->fileInfo(p_val);
	m_assetFileModel->setRootPath(fileInfo.filePath());
	ui.listView_fileBrowser->setRootIndex(m_assetFileModel->index(fileInfo.filePath()));

	SetFolderSpecificFilters(fileInfo.baseName());
	//Clear search bar when selecting a new folder. 2014-05-08: Only local search is available.
	ui.lineEdit_assetSearch->clear();
	SearchLineChanged("");
}

//Double click in list view
void AssetManagerWidget::FileSelected( const QModelIndex& p_val )
{
	QFileInfo fileInfo = m_assetFileModel->fileInfo(p_val);
	if(fileInfo.isDir()) //Doubleclicked a folder! Navigate YO!
	{
		m_assetFileModel->setRootPath(fileInfo.filePath());
		ui.listView_fileBrowser->setRootIndex(m_assetFileModel->index(fileInfo.filePath()));

		ui.treeView_assetFileBrowser->setCurrentIndex(m_assetFolderModel->index(fileInfo.filePath()));
		SetFolderSpecificFilters(fileInfo.baseName());
	}
}

//Navigate back in list view
void AssetManagerWidget::NavigateBack()
{
	
	QDir temp(m_assetFileModel->rootPath());
	//Check if we back up too much
	if(temp.dirName() == "Assets")
		return;

	if(!temp.cdUp())
	{
		g_engineContext.m_logger->LogText(LogTag::TOOLS, LogLevel::DEBUG_PRINT, "Navigating back is not really a good idea any more...");
	}
	else
	{
		//New backed-up path!
		QString newPath = temp.absolutePath();

		m_assetFileModel->setRootPath(newPath);
		ui.listView_fileBrowser->setRootIndex(m_assetFileModel->index(newPath));
		ui.treeView_assetFileBrowser->setCurrentIndex(m_assetFolderModel->index(newPath));
	}
}

//Navigate forward in list view
void AssetManagerWidget::NavigateForward()
{

}

void AssetManagerWidget::IconSizeChanged( int p_val )
{
	//In slider is all the way to the left we switch to list mode!
	if(p_val == ui.horizontalSlider_icon->minimum())
	{
		ui.listView_fileBrowser->setViewMode(QListView::ListMode);
		ui.listView_fileBrowser->setIconSize(QSize(15, 15));
		ui.listView_fileBrowser->setGridSize(QSize(15, 15));	
	}
	else
	{
		ui.listView_fileBrowser->setViewMode(QListView::IconMode);
		ui.listView_fileBrowser->setIconSize(QSize(p_val, p_val));	
		ui.listView_fileBrowser->setGridSize(QSize(p_val + 10, p_val + 25));	
		ui.listView_fileBrowser->setWordWrap(true);
	}
	//Reset drag mode(It got removed when changing view mode)
	ui.listView_fileBrowser->setDragDropMode(QAbstractItemView::DragOnly);
}

void AssetManagerWidget::TreeListContextMenu(const QPoint& p_val)
{
	QModelIndex temp = ui.listView_fileBrowser->indexAt(p_val);

	if(temp.isValid())
	{
		QFileInfo fileInfo = m_assetFileModel->fileInfo(temp);

		//File right clicked
		if(fileInfo.isFile())
		{
			QAction* selectedAction = m_fileContextMenu->exec(ui.listView_fileBrowser->mapToGlobal( p_val ));
		
			//No action selected
			if(selectedAction == nullptr)
				return;

			if(selectedAction->text() == "Open externally")
			{
				if(fileInfo.suffix().compare("particle") == 0) //Doubleclicked a .particle-file. Opens up the particle editor!
				{
					//Start the Particle editor from the same folder. Alternative is to remove this if-statement and let Qt open the particle editor.
					std::string particleFileInfo = fileInfo.filePath().toStdString();
					//Enclose .exe-file path in quotes and enclose argument-path in quotes and then enclose the whole thing in quotes! Windows Magic right here :D
					std::string particleEditorPath = "\"\"" + g_engineContext.m_resourceManager->GetWorkingDirectory() + "ParticleEditor.exe" + "\" \"" + particleFileInfo + "\"\"";
					system(particleEditorPath.c_str());
					Utils::Write("Particle editor started!");
				}
				else
				{
					//This opens a file with the default program. If no program is selected it will prompt the user to find a program. Very nice... very nice indeed!
					QString temp = "file:///" + fileInfo.filePath();
					QDesktopServices::openUrl(QUrl(temp, QUrl::TolerantMode)); //Don't know if QDesktopServices is included in our external libs. If this is causing compile problems, comment it out!
				}
			}
		}
	}
	else
	{
		//No item is right clicked
	}
}

void AssetManagerWidget::SetFolderSpecificFilters( const QString& p_folderName )
{
	m_currentFilter.clear();

	if(p_folderName == "Levels")
	{
		m_currentFilter << ".world";
	}
	else if(p_folderName == "Audio")
	{
		m_currentFilter << ".mp3" << ".wav" << ".flac" << ".ogg";
	}

	//Update filter
	SearchLineChanged("");
}

void AssetManagerWidget::FolderLoaded( const QString & p_path )
{
	//ui.treeView_assetFileBrowser->expandAll();
	//ui.treeView_assetFileBrowser->collapseAll();
}

void AssetManagerWidget::CollapseAll()
{
	ui.treeView_assetFileBrowser->collapseAll();
}

void AssetManagerWidget::ExpandAll()
{
	//Utils::RunWithProgressBar(QtConcurrent::run(ui.treeView_assetFileBrowser, &QTreeView::expandAll));
	ui.treeView_assetFileBrowser->expandAll();
}

void AssetManagerWidget::FileClicked( const QModelIndex& p_val )
{
	QFileInfo fileInfo = m_assetFileModel->fileInfo(p_val);
	
	//File clicked
	ui.label_fileName->setText(fileInfo.fileName());
	ui.label_sizeName->setText(QString::number((fileInfo.size() / 1000) + 1) + " KB");
	ui.label_createdName->setText(fileInfo.created().toString());

	//Reset preview image
	ui.widget_pictureFrame->setStyleSheet("");
	ui.widget_pictureFrame->setPixmap(QPixmap());

	if(fileInfo.isFile())
	{
		if(fileInfo.suffix() == "world")
		{
			//Level file selected
			QString image = fileInfo.dir().path() + "/" + fileInfo.completeBaseName() + ".png";
			QFileInfo file(image);
			if(file.exists())
			{
				ui.widget_pictureFrame->setStyleSheet("border-image: url(" + image + ")");
			}
			else
			{
				//If .png is not available, check if .jpg is
				image = fileInfo.dir().path() + "/" + fileInfo.completeBaseName() + ".jpg";
				QFileInfo fileJPG(image);
				if(fileJPG.exists())
				{
					ui.widget_pictureFrame->setStyleSheet("border-image: url(" + image + ")");
				}
			}
			
		}
		else if(fileInfo.suffix() == "png" || fileInfo.suffix() == "jpg")
		{
			ui.widget_pictureFrame->setStyleSheet("border-image: url(" + fileInfo.filePath() + ")");
		}
		else if(fileInfo.suffix() == "dds")
		{
			QImageReader hello(fileInfo.filePath());

			QFile imagefile(fileInfo.filePath());

			if(imagefile.open(QIODevice::ReadOnly))
				Utils::Write("Could not open DDS-file");

			m_ddsHandler->setDevice(&imagefile);
			m_ddsHandler->setFormat(QByteArray());

			if(!m_ddsHandler->canRead())
			{
				Utils::Write("DDS handler cannot read!");
				return;
			}
			else
			{
				QImage image;
				if(m_ddsHandler->read(&image))
				{
					Utils::Write("DDS loaded succesfully");
				}
				else
				{
					Utils::Write("DDS NOT loaded!");
				}
				imagefile.close();
				
				Utils::Write(QString::number(m_ddsHandler->imageCount()));
				Utils::Write("DDS x,y: " + QString::number(image.size().width()) + ", " + QString::number(image.size().height()));
				ui.widget_pictureFrame->setPixmap(QPixmap::fromImage(image));
			}
		}
	}
}
