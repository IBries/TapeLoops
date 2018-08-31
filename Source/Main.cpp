/****************************//**
 *	@file Main.cpp
 *	@author Isaac Bries
 *	@date Created:	8/01/2018
 *	@date Edited:	8/01/2018
 *******************************/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

 /************************************************************//**
 *	@class TapeLoopsApplication
 *
 *	@brief Contains basic startup code for a JUCE application

 *	Does all the boring nitty-gritty things needed to get a
 *	basic application up and running. Thanks, JUCE.
 ***************************************************************/
class TapeLoopsApplication  : public JUCEApplication
{
public:
	/****************************//**
	 *	@brief Default Constructor
	 *******************************/
    TapeLoopsApplication()
	{
	}

	/****************************************//**
	 *	@brief Gets the name of the application
	 *
	 *	@return the name of the application
	 ********************************************/
	const String getApplicationName() override
	{
		return ProjectInfo::projectName;
	}

	/********************************************//**
	 *	@brief Gets the version of the application
	 *
	 *	@return the version of the application
	 ***********************************************/
    const String getApplicationVersion() override
	{
		return ProjectInfo::versionString;
	}

	/****************************************************************//**
	 *	@brief flag for having multiple instances of this application
	 *
	 *	@return true
	 *				if more than one instance is allowed
	 *			false
	 *				otherwise
	 *******************************************************************/
	bool moreThanOneInstanceAllowed() override
	{
		return true;
	}

	/****************************************************************//**
	 *	@brief This method is where you should put your application's
	 *		   initialisation code.
	 *
	 *	@param[in]	&commandLine
	 *					a pointer to the command line arguments
	 *
	 *	@return void
	 *******************************************************************/
	void initialise (const String& commandLine) override
    {
        mainWindow.reset (new MainWindow (getApplicationName()));
    }

	/****************************************************//**
	 *	@brief Add your application's shutdown code here.
	 *
	 *	@return void
	 *******************************************************/
    void shutdown() override
    {		
        mainWindow = nullptr;	// deletes our window
    }

	/********************************************************************//**
	 *	@brief This is called when the app is being asked to quit: you
	 *		   can ignore this request and let the app carry on running,
	 *		   or call quit() to allow the app to close.
	 *
	 *	@return void
	 ***********************************************************************/
    void systemRequestedQuit() override
    {
        quit();
    }

	/********************************************************************//**
	 *	@brief When another instance of the app is launched while this one
	 *		   is running, this method is invoked, and the commandLine
	 *		   parameter tells you what the other instance's command-line
	 *		   arguments were.
	 *
	 *	@param[in]	&commandLine
	 *					a pointer to the command line arguments
	 *
	 *	@return void
	 ***********************************************************************/
    void anotherInstanceStarted (const String& commandLine) override
    {
    }

    /************************************************************************//**
	 *	@class MainWindow
	 *
	 *	@brief Implements the application window
	 *
     *  This class implements the desktop window that contains an instance of
     *  our MainComponent class.
     ***************************************************************************/
    class MainWindow    : public DocumentWindow
    {
    public:
		/****************************************//**
		 *	@brief	Default Constructor
		 *
		 *	@param[in] name
		 *			   the name of the application
		 *******************************************/
        MainWindow (String name)  :
			DocumentWindow (name,
							Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),
							DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);
            setResizable (true, true);

            centreWithSize (getWidth(), getHeight());
            setVisible (true);
        }

		/********************************************************************//**
		 *	@brief	This is called when the user tries to close this window.
		 *			Here, we'll just ask the app to quit when this happens, but	*
		 *			you can change this to do whatever you need.
		 *
		 *	@return void
		 ***********************************************************************/
        void closeButtonPressed() override
		{
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;		///< Scoped pointer to our Main Window
};

// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (TapeLoopsApplication)
