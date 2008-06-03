/*
  ==============================================================================

   JUCE library : Starting point code, v1.26
   Copyright 2005 by Julian Storer. [edited by haydxn, 3rd April 2007]

  ------------------------------------------------------------------------------

  MainComponent.h :

  This file defines the behaviour of the application. The main part of the
  program that the user interacts with IS this MainComponent object. It is
  placed within the MainAppWindow instance, and so exists at whatever size
  the window provides in its content area.

  All of the widgets and controls that your main program window will display
  will be on this component (either directly or somewhere down the children
  hierarchy).

  ------------------------------------------------------------------------------

  Please feel free to do whatever you like with this code, bearing in mind that
  it's not guaranteed to be bug-free!

  ==============================================================================
*/
#ifndef _MAINCOMPONENT_H_
#define _MAINCOMPONENT_H_

#include "juce.h"

class MainComponent  : public Component      // Here we specify any base classes that
                                 // give this type predefined characteristics.
                                 // Naturally, this is a Component, but we could
                                 // also inherit other qualities. For example, if
                                 // we want to respond to button presses, we can
                                 // inherit 'ButtonListener', by changing it thus:
               // e.g.
               // public Component,
               // public ButtonListener
                           
               // (Notice that they're separated by a comma, and nothing comes after the
               //  final one in the list - i.e. no semicolon, because the next character
               //  must be the '{' denoting the start of the class body).

               // We can inherit many different classes from Juce (or classes we make
               // ourselves), but be aware that some base classes require you to define
               // some function bodies before it will allow your app to compile. These
               // functions are called 'pure virtual' functions - an example would be
               // 'buttonClicked' in ButtonListener. Some base classes will provide many
               // other virtual functions - not just pure virtual ones - which you can
               // define if you choose, but you do not have to do so.
{
private:
    //==============================================================================

   // Here are some members that are useful to have in any application...

    TooltipWindow tooltipWindow;   // To add tooltips to an application, you
                           // just need to create one of these and leave it
                           // there to do its work.

   // Your app will obviously have some kind of member variables, and so this
   // is the place you can declare them. For example, if you wanted to have a
   // widget or some component you've made, you'd have a pointer for it here;
   // you'd then instantiate the object in the constructor, and use the pointer
   // to access it elsewhere (e.g. to position it, update it, or respond to it).

   // e.g.
   // TextButton* myButton;

public:
    //==============================================================================
    MainComponent ()
    {
      // This is where the main component is created, so we initialise and
      // configure it according to our needs.

      // One thing that covers is creating any widgets and components we want to
      // display. Also, if any widgets will need responding to, we must hook them up
      // to their listeners here too (and it's likely that this class itself will be
      // the listener in question, providing we've inherited the appropriate class!)

      // Create and add the rest of your components here!
      // e.g.
      //myButton = new TextButton (T("my button"), T("Click me!"));
      //addAndMakeVisible (myButton);
    }

    ~MainComponent ()
    {
      // Be sure to destroy any objects you've created using 'new' here. If your objects
      // are on the stack (i.e. they were created without pointers or the 'new' operator,
      // then they die automatically. If you've created them
      // manually on the heap (for example, if you've got a pointer and you've created a new
      // object for it) then it must be deleted.

      // However, juce has a nice neat function that will destroy all Components that have
      // been added to a Component...

      deleteAllChildren();   // This will remove all the contained components, and delete
                        // the objects for you.
    }

    //==============================================================================
    void resized ()
    {
      // This is called whenever this component's size changes. We could respond
      // to this in a number of ways, but the most obvious thing to do is reposition
      // all our widgets, using their 'setBounds()' function.
      // It's nice to position them relative to the size of this Component. That means
      // making use of the 'getWidth()' and 'getHeight()' functions to determine where
      // to put them and how big they should be.
    }

   void paint (Graphics& g)
   {
      // This does any drawing required on this Component's face. You can simply
      // call functions on the provided Graphics object - although you may want to
      // make use of 'getWidth()' and 'getHeight()' (members of this Component) to
      // make sure you're drawing in the right place!
   }


    //==============================================================================
};

#endif//_MAINCOMPONENT_H_ 
