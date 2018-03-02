// Copyright 2011 Boris Kogan (boris@thekogans.net)
//
// This file is part of libthekogans_mex_3ds_io_command.
//
// libthekogans_mex_3ds_io_command is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libthekogans_mex_3ds_io_command is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libthekogans_mex_3ds_io_command. If not, see <http://www.gnu.org/licenses/>.

#include <numeric>
//#include <boost/assign.hpp>
#include "thekogans/mex/command/SimpleTypeCommands.h"
#include "thekogans/mex/3ds/ext/ViewLayout.h"
#include "thekogans/mex/3ds/io/command/BlasSimpleTypeCommands.h"
#include "thekogans/mex/3ds/io/command/ViewCommands.h"
#include "thekogans/mex/3ds/io/command/ViewLayoutCommands.h"

//using namespace boost::assign;
using namespace thekogans::mex::command;

namespace thekogans {
    namespace mex {
        namespace _3ds {
            namespace io {
                namespace command {

                    bool ViewLayoutSetSizeCommand::Execute () {
                        ExecuteAndAddCommand (
                            Command::UniquePtr (
                                new SizeSetCommand (viewLayout.size, size)));
                        // Update view bounds based on layout currStyle.
                        switch (viewLayout.layout.currStyle) {
                            case io::ViewLayout::Layout::Single: {
                                const ext::ViewLayout::Dimension x (
                                    size.x, size.width, 1);
                                const ext::ViewLayout::Dimension y (
                                    size.y, size.height, 1);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[0].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                y.GetViewStart (0),
                                                x.GetViewSize (0),
                                                y.GetViewSize (0)))));
                                break;
                            }
                            case io::ViewLayout::Layout::TwoVert: {
                                const ext::ViewLayout::Dimension x (
                                    size.x, size.width, 2);
                                const ext::ViewLayout::Dimension y (
                                    size.y, size.height, 1);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[0].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                y.GetViewStart (0),
                                                x.GetViewSize (0),
                                                y.GetViewSize (0)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[1].size, 
                                            blas::Size (
                                                x.GetViewStart (1),
                                                y.GetViewStart (0),
                                                x.GetViewSize (1),
                                                y.GetViewSize (0)))));
                                break;
                            }
                            case io::ViewLayout::Layout::TwoHorz: {
                                const ext::ViewLayout::Dimension x (
                                    size.x, size.width, 1);
                                const ext::ViewLayout::Dimension y (
                                    size.y, size.height, 2);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[0].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                y.GetViewStart (1),
                                                x.GetViewSize (0),
                                                y.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[1].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                y.GetViewStart (0),
                                                x.GetViewSize (0),
                                                y.GetViewSize (0)))));
                                break;
                            }
                            case io::ViewLayout::Layout::Four: {
                                const ext::ViewLayout::Dimension x (size.x, size.width, 2);
                                const ext::ViewLayout::Dimension y (size.y, size.height, 2);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[0].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                y.GetViewStart (1),
                                                x.GetViewSize (0),
                                                y.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[1].size, 
                                            blas::Size (
                                                x.GetViewStart (1),
                                                y.GetViewStart (1),
                                                x.GetViewSize (1),
                                                y.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[2].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                y.GetViewStart (0),
                                                x.GetViewSize (0),
                                                y.GetViewSize (0)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[3].size, 
                                            blas::Size (
                                                x.GetViewStart (1),
                                                y.GetViewStart (0),
                                                x.GetViewSize (1),
                                                y.GetViewSize (0)))));
                                break;
                            }
                            case io::ViewLayout::Layout::ThreeLeft: {
                                const ext::ViewLayout::Dimension x (
                                    size.x, size.width, 2);
                                const ext::ViewLayout::Dimension yLeft (
                                    size.y, size.height, 2);
                                const ext::ViewLayout::Dimension yRight (
                                    size.y, size.height, 1);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[0].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                yLeft.GetViewStart (1),
                                                x.GetViewSize (0),
                                                yLeft.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[1].size, 
                                            blas::Size (
                                                x.GetViewStart (1),
                                                yRight.GetViewStart (0),
                                                x.GetViewSize (1),
                                                yRight.GetViewSize (0)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[2].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                yLeft.GetViewStart (0),
                                                x.GetViewSize (0),
                                                yLeft.GetViewSize (0)))));
                                break;
                            }
                            case io::ViewLayout::Layout::ThreeBottom: {
                                const ext::ViewLayout::Dimension xTop (
                                    size.x, size.width, 1);
                                const ext::ViewLayout::Dimension xBottom (
                                    size.x, size.width, 2);
                                const ext::ViewLayout::Dimension y (
                                    size.y, size.height, 2);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[0].size, 
                                            blas::Size (
                                                xTop.GetViewStart (0),
                                                y.GetViewStart (1),
                                                xTop.GetViewSize (0),
                                                y.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[1].size, 
                                            blas::Size (
                                                xBottom.GetViewStart (0),
                                                y.GetViewStart (0),
                                                xBottom.GetViewSize (0),
                                                y.GetViewSize (0)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[2].size, 
                                            blas::Size (
                                                xBottom.GetViewStart (1),
                                                y.GetViewStart (0),
                                                xBottom.GetViewSize (1),
                                                y.GetViewSize (0)))));
                                break;
                            }
                            case io::ViewLayout::Layout::ThreeRight: {
                                const ext::ViewLayout::Dimension x (
                                    size.x, size.width, 2);
                                const ext::ViewLayout::Dimension yLeft (
                                    size.y, size.height, 1);
                                const ext::ViewLayout::Dimension yRight (
                                    size.y, size.height, 2);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[0].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                yLeft.GetViewStart (0),
                                                x.GetViewSize (0),
                                                yLeft.GetViewSize (0)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[1].size, 
                                            blas::Size (
                                                x.GetViewStart (1),
                                                yRight.GetViewStart (1),
                                                x.GetViewSize (1),
                                                yRight.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[2].size, 
                                            blas::Size (
                                                x.GetViewStart (1),
                                                yRight.GetViewStart (0),
                                                x.GetViewSize (1),
                                                yRight.GetViewSize (0)))));
                                break;
                            }
                            case io::ViewLayout::Layout::ThreeTop: {
                                const ext::ViewLayout::Dimension xTop (
                                    size.x, size.width, 2);
                                const ext::ViewLayout::Dimension xBottom (
                                    size.x, size.width, 1);
                                const ext::ViewLayout::Dimension y (
                                    size.y, size.height, 2);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[0].size, 
                                            blas::Size (
                                                xTop.GetViewStart (0),
                                                y.GetViewStart (1),
                                                xTop.GetViewSize (0),
                                                y.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[1].size, 
                                            blas::Size (
                                                xTop.GetViewStart (1),
                                                y.GetViewStart (1),
                                                xTop.GetViewSize (1),
                                                y.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[2].size, 
                                            blas::Size (
                                                xBottom.GetViewStart (0),
                                                y.GetViewStart (0),
                                                xBottom.GetViewSize (0),
                                                y.GetViewSize (0)))));
                                break;
                            }
                            case io::ViewLayout::Layout::ThreeVert: {
                                const ext::ViewLayout::Dimension x (
                                    size.x, size.width, 3);
                                const ext::ViewLayout::Dimension y (
                                    size.y, size.height, 1);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[0].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                y.GetViewStart (0),
                                                x.GetViewSize (0),
                                                y.GetViewSize (0)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[1].size, 
                                            blas::Size (
                                                x.GetViewStart (1),
                                                y.GetViewStart (0),
                                                x.GetViewSize (1),
                                                y.GetViewSize (0)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[2].size, 
                                            blas::Size (
                                                x.GetViewStart (2),
                                                y.GetViewStart (0),
                                                x.GetViewSize (2),
                                                y.GetViewSize (0)))));
                                break;
                            }
                            case io::ViewLayout::Layout::ThreeHorz: {
                                const ext::ViewLayout::Dimension x (
                                    size.x, size.width, 1);
                                const ext::ViewLayout::Dimension y (
                                    size.y, size.height, 3);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[0].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                y.GetViewStart (2),
                                                x.GetViewSize (0),
                                                y.GetViewSize (2)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[1].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                y.GetViewStart (1),
                                                x.GetViewSize (0),
                                                y.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[2].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                y.GetViewStart (0),
                                                x.GetViewSize (0),
                                                y.GetViewSize (0)))));
                                break;
                            }
                            case io::ViewLayout::Layout::FourLeft: {
                                ext::ViewLayout::Dimension::Coverage coverage;
                                //coverage += util::Fraction (1, 3), util::Fraction (2, 3);
                                coverage.push_back (util::Fraction (1, 3));
                                coverage.push_back (util::Fraction (2, 3));
                                const ext::ViewLayout::Dimension x (
                                    size.x, size.width, 2, coverage);
                                const ext::ViewLayout::Dimension yLeft (
                                    size.y, size.height, 3);
                                const ext::ViewLayout::Dimension yRight (
                                    size.y, size.height, 1);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[0].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                yLeft.GetViewStart (2),
                                                x.GetViewSize (0),
                                                yLeft.GetViewSize (2)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[1].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                yLeft.GetViewStart (1),
                                                x.GetViewSize (0),
                                                yLeft.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[2].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                yLeft.GetViewStart (0),
                                                x.GetViewSize (0),
                                                yLeft.GetViewSize (0)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[3].size, 
                                            blas::Size (
                                                x.GetViewStart (1),
                                                yRight.GetViewStart (0),
                                                x.GetViewSize (1),
                                                yRight.GetViewSize (0)))));
                                break;
                            }
                        #if EXTENDED_VIEW_LAYOUT
                            case io::ViewLayout::Layout::FourBottom: {
                                const ext::ViewLayout::Dimension xTop (
                                    size.x, size.width, 1);
                                const ext::ViewLayout::Dimension xBottom (
                                    size.x, size.width, 3);
                                ext::ViewLayout::Dimension::Coverage coverage;
                                //coverage += util::Fraction (1, 3), util::Fraction (2, 3);
                                coverage.push_back (util::Fraction (1, 3));
                                coverage.push_back (util::Fraction (2, 3));
                                const ext::ViewLayout::Dimension y (
                                    size.y, size.height, 2, coverage);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[0].size, 
                                            blas::Size (
                                                xTop.GetViewStart (0),
                                                y.GetViewStart (1),
                                                xTop.GetViewSize (0),
                                                y.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[1].size, 
                                            blas::Size (
                                                xBottom.GetViewStart (0),
                                                y.GetViewStart (0),
                                                xBottom.GetViewSize (0),
                                                y.GetViewSize (0)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[2].size, 
                                            blas::Size (
                                                xBottom.GetViewStart (1),
                                                y.GetViewStart (0),
                                                xBottom.GetViewSize (1),
                                                y.GetViewSize (0)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[3].size, 
                                            blas::Size (
                                                xBottom.GetViewStart (2),
                                                y.GetViewStart (0),
                                                xBottom.GetViewSize (2),
                                                y.GetViewSize (0)))));
                                break;
                            }
                        #endif // EXTENDED_VIEW_LAYOUT
                            case io::ViewLayout::Layout::FourRight: {
                                ext::ViewLayout::Dimension::Coverage coverage;
                                //coverage += util::Fraction (2, 3), util::Fraction (1, 3);
                                coverage.push_back (util::Fraction (2, 3));
                                coverage.push_back (util::Fraction (1, 3));
                                const ext::ViewLayout::Dimension x (
                                    size.x, size.width, 2, coverage);
                                const ext::ViewLayout::Dimension yLeft (
                                    size.y, size.height, 1);
                                const ext::ViewLayout::Dimension yRight (
                                    size.y, size.height, 3);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[0].size, 
                                            blas::Size (
                                                x.GetViewStart (0),
                                                yLeft.GetViewStart (0),
                                                x.GetViewSize (0),
                                                yLeft.GetViewSize (0)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[1].size, 
                                            blas::Size (
                                                x.GetViewStart (1),
                                                yRight.GetViewStart (2),
                                                x.GetViewSize (1),
                                                yRight.GetViewSize (2)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[2].size, 
                                            blas::Size (
                                                x.GetViewStart (1),
                                                yRight.GetViewStart (1),
                                                x.GetViewSize (1),
                                                yRight.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[3].size, 
                                            blas::Size (
                                                x.GetViewStart (1),
                                                yRight.GetViewStart (0),
                                                x.GetViewSize (1),
                                                yRight.GetViewSize (0)))));
                                break;
                            }
                        #if EXTENDED_VIEW_LAYOUT
                            case io::ViewLayout::Layout::FourTop: {
                                const ext::ViewLayout::Dimension xTop (
                                    size.x, size.width, 3);
                                const ext::ViewLayout::Dimension xBottom (
                                    size.x, size.width, 1);
                                ext::ViewLayout::Dimension::Coverage coverage;
                                //coverage += util::Fraction (2, 3), util::Fraction (1, 3);
                                coverage.push_back (util::Fraction (2, 3));
                                coverage.push_back (util::Fraction (1, 3));
                                const ext::ViewLayout::Dimension y (
                                    size.y, size.height, 2, coverage);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[0].size, 
                                            blas::Size (
                                                xTop.GetViewStart (0),
                                                y.GetViewStart (1),
                                                xTop.GetViewSize (0),
                                                y.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[1].size, 
                                            blas::Size (
                                                xTop.GetViewStart (1),
                                                y.GetViewStart (1),
                                                xTop.GetViewSize (1),
                                                y.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[2].size, 
                                            blas::Size (
                                                xTop.GetViewStart (2),
                                                y.GetViewStart (1),
                                                xTop.GetViewSize (2),
                                                y.GetViewSize (1)))));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SizeSetCommand (
                                            viewLayout[3].size, 
                                            blas::Size (
                                                xBottom.GetViewStart (0),
                                                y.GetViewStart (0),
                                                xBottom.GetViewSize (0),
                                                y.GetViewSize (0)))));
                                break;
                            }
                        #endif // EXTENDED_VIEW_LAYOUT
                            default:
                                // FIXME: throw something appropriate
                                assert (0);
                                break;
                        }
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ViewLayoutSetSizeCommand)

                    bool ViewLayoutSetCurrViewCommand::Execute () {
                        if (currViewIndex != viewLayout.layout.currViewIndex) {
                            assert (currViewIndex < 5);
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ui16SetCommand (
                                        viewLayout.layout.currViewIndex,
                                        currViewIndex)));
                            return true;
                        }
                        return false;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ViewLayoutSetCurrViewCommand)

                    bool ViewLayoutToggleMaximizedCommand::Execute () {
                        if (!viewLayout.layout.maximized) {
                            if (viewLayout.layout.currViewIndex > 0) {
                                assert (viewLayout.views3.size () == 5);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SimpleTypeSetCommand<io::View> (
                                            viewLayout[4],
                                            viewLayout[0])));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SimpleTypeSetCommand<io::View> (
                                            viewLayout[0],
                                            viewLayout[viewLayout.layout.currViewIndex])));
                            }
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ui16SetCommand (
                                        viewLayout.layout.lastStyle,
                                        viewLayout.layout.currStyle)));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ui16SetCommand (
                                        viewLayout.layout.currStyle,
                                        io::ViewLayout::Layout::Single)));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ui16SetCommand (
                                        viewLayout.layout.lastViewIndex,
                                        viewLayout.layout.currViewIndex)));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ui16SetCommand (
                                        viewLayout.layout.currViewIndex,
                                        0)));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new boolSetCommand (
                                        viewLayout.layout.maximized,
                                        true)));
                        }
                        else {
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new boolSetCommand (
                                        viewLayout.layout.maximized,
                                        false)));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ui16SetCommand (
                                        viewLayout.layout.currStyle,
                                        viewLayout.layout.lastStyle)));
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ui16SetCommand (
                                        viewLayout.layout.currViewIndex,
                                        viewLayout.layout.lastViewIndex)));
                            if (viewLayout.layout.currViewIndex > 0) {
                                assert (viewLayout.views3.size () == 5);
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SimpleTypeSetCommand<io::View> (
                                            viewLayout[viewLayout.layout.currViewIndex],
                                            viewLayout[0])));
                                ExecuteAndAddCommand (
                                    Command::UniquePtr (
                                        new SimpleTypeSetCommand<io::View> (
                                            viewLayout[0],
                                            viewLayout[4])));
                            }
                        }
                        ExecuteAndAddCommand (
                            Command::UniquePtr (
                                new ViewLayoutSetSizeCommand (
                                    viewLayout,
                                    viewLayout.size)));
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ViewLayoutToggleMaximizedCommand)

                    bool ViewLayoutZoomInCommand::Execute () {
                        for (std::size_t i = 0,
                                count = viewLayout.GetViewCount (); i < count; ++i) {
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ViewZoomInCommand (viewLayout[i])));
                        }
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ViewLayoutZoomInCommand)

                    bool ViewLayoutZoomOutCommand::Execute () {
                        for (std::size_t i = 0,
                                count = viewLayout.GetViewCount (); i < count; ++i) {
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ViewZoomOutCommand (viewLayout[i])));
                        }
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ViewLayoutZoomOutCommand)

                    bool ViewLayoutZoomBoundCommand::Execute () {
                        for (std::size_t i = 0,
                                count = viewLayout.GetViewCount (); i < count; ++i) {
                            ExecuteAndAddCommand (
                                Command::UniquePtr (
                                    new ViewZoomBoundCommand (viewLayout[i], bound)));
                        }
                        return true;
                    }

                    THEKOGANS_MEX_COMMAND_IMPLEMENT_COMMAND (ViewLayoutZoomBoundCommand)

                } // namespace command
            } // namespace io
        } // namespace _3ds
    } // namespace mex
} // namespace thekogans
