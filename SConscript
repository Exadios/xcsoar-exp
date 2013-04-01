# Copyright_License {
#
#  XCSoar Glide Computer - http://www.xcsoar.org/
#  Copyright (C) 2000-2012 The XCSoar Project
#  A detailed list of copyright holders can be found in the file "AUTHORS".
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#}

Import ('*')

#for item in sorted(env.Dictionary().items()):
#    print "construction variable = '%s', value = '%s'" % item

bd = topsrcdir + 'Engine/Airspace/'
e.StaticLibrary('airspace',
                [ topsrcdir + '/Engine/Util/AircraftStateFilter.cpp',
                  bd + 'AirspacesTerrain.cpp',
                 	bd + 'Airspace.cpp',
                  bd + 'AirspaceAltitude.cpp',
                  bd + 'AirspaceAircraftPerformance.cpp',
                  bd + 'AbstractAirspace.cpp',
                  bd + 'AirspaceCircle.cpp',
                  bd + 'AirspacePolygon.cpp',
                  bd + 'Airspaces.cpp',
                  bd + 'AirspaceIntersectSort.cpp',
                  bd + 'AirspaceNearestSort.cpp',
                  bd + 'AirspaceSoonestSort.cpp',
                  bd + 'Predicate/AirspacePredicate.cpp',
                  bd + 'Predicate/AirspacePredicateAircraftInside.cpp',
                  bd + 'Predicate/AirspacePredicateHeightRange.cpp',
                  bd + 'AirspaceVisitor.cpp',
                  bd + 'AirspaceIntersectionVisitor.cpp',
                  bd + 'AirspaceWarningConfig.cpp',
                  bd + 'AirspaceWarningManager.cpp',
                  bd + 'AirspaceWarning.cpp',
                  bd + 'AirspaceSorter.cpp' ])

bd = topsrcdir + 'Audio/'
e.StaticLibrary('audio', [ bd + 'ToneSynthesiser.cpp',
                           bd + 'VarioSynthesiser.cpp',
                           bd + 'PCMPlayer.cpp' ])

e.StaticLibrary('contest',
                [ topsrcdir + '/Engine/Contest/Settings.cpp',
                  topsrcdir + '/Engine/Contest/ContestManager.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/Contests.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/AbstractContest.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/ContestDijkstra.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/DMStQuad.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/OLCLeague.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/OLCSprint.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/OLCClassic.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/OLCTriangle.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/OLCFAI.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/OLCPlus.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/DMStQuad.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/XContestFree.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/XContestTriangle.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/OLCSISAT.cpp',
                  topsrcdir + '/Engine/Contest/Solvers/NetCoupe.cpp' ])

event_srcs = [ topsrcdir + '/Event/Idle.cpp',
	             topsrcdir + '/Event/DelayedNotify.cpp',
	             topsrcdir + '/Event/Notify.cpp' ]
if int(enable_sdl) == 1:
  event_srcs = event_srcs + [ topsrcdir + '/Event/SDL/Timer.cpp',
	                            topsrcdir + '/Event/SDL/Loop.cpp',
	                            topsrcdir + '/Event/SDL/Queue.cpp' ]
e.StaticLibrary('event', event_srcs)

bd = topsrcdir + 'Geo/'
e.StaticLibrary('geo', [ bd + 'ConvexHull/GrahamScan.cpp',
                         bd + 'ConvexHull/PolygonInterior.cpp',
                         bd + 'Memento/DistanceMemento.cpp',
                         bd + 'Memento/GeoVectorMemento.cpp',
                         bd + 'Flat/TaskProjection.cpp',
                         bd + 'Flat/FlatBoundingBox.cpp',
                         bd + 'Flat/FlatGeoPoint.cpp',
                         bd + 'Flat/FlatRay.cpp',
                         bd + 'Flat/FlatPoint.cpp',
                         bd + 'Flat/FlatEllipse.cpp',
                         bd + 'Flat/FlatLine.cpp',
                         bd + 'Math.cpp',
                         bd + 'GeoPoint.cpp',
                         bd + 'GeoVector.cpp',
                         bd + 'GeoBounds.cpp',
                         bd + 'GeoClip.cpp',
                         bd + 'SearchPoint.cpp',
                         bd + 'SearchPointVector.cpp',
                         bd + 'GeoEllipse.cpp',
                         bd + 'UTM.cpp' ])

bd = topsrcdir + 'Engine/GlideSolvers/'
e.StaticLibrary('glide', [ bd + 'GlideSettings.cpp',
                           bd + 'GlideState.cpp',
                           bd + 'GlidePolar.cpp',
                           bd + 'PolarCoefficients.cpp',
                           bd + 'GlideResult.cpp',
                           bd + 'MacCready.cpp' ])
bd = topsrcdir + 'Net/'
e.ParseConfig("pkg-config libcurl --cflags --libs")
e.StaticLibrary('net', [ bd + 'CURL/Multi.cpp',
                         bd + 'CURL/Session.cpp',
                         bd + 'CURL/Request.cpp',
                         bd + 'CURL/Init.cpp',
                         bd + 'DownloadManager.cpp',
                         bd + 'ToFile.cpp',
                         bd + 'ToBuffer.cpp' ])

e.StaticLibrary('os', [ topsrcdir + '/OS/Clock.cpp',
                        topsrcdir + '/OS/SocketAddress.cpp',
                        topsrcdir + '/OS/SocketDescriptor.cpp',
                        topsrcdir + '/OS/FileDescriptor.cpp',
                        topsrcdir + '/OS/FileMapping.cpp',
                        topsrcdir + '/OS/FileUtil.cpp',
                        topsrcdir + '/OS/PathName.cpp',
                        topsrcdir + '/OS/SystemLoad.cpp',
                        topsrcdir + '/OS/Poll.cpp',
                        topsrcdir + '/OS/EventPipe.cpp',
                        topsrcdir + '/OS/LogError.cpp' ])

bd = topsrcdir + 'Engine/Route/'
e.StaticLibrary('route', [ bd + 'Config.cpp',
                           bd + 'RoutePlanner.cpp',
                           bd + 'AirspaceRoute.cpp',
                           bd + 'TerrainRoute.cpp',
                           bd + 'RouteLink.cpp',
                           bd + 'RoutePolar.cpp',
                           bd + 'RoutePolars.cpp',
                           bd + 'FlatTriangleFan.cpp',
                           bd + 'FlatTriangleFanTree.cpp',
                           bd + 'ReachFan.cpp' ])

bd = topsrcdir + 'Engine/Task/'
e.StaticLibrary('task', [ bd + 'Shapes/FAITriangleRules.cpp',
                          bd + 'Shapes/FAITriangleArea.cpp',
                          bd + 'Shapes/FAITriangleTask.cpp',
                          bd + 'Shapes/FAITrianglePointValidator.cpp',
                          bd + 'TaskBehaviour.cpp',
                          bd + 'TaskManager.cpp',
                          bd + 'AbstractTask.cpp',
                          bd + 'Ordered/StartConstraints.cpp',
                          bd + 'Ordered/FinishConstraints.cpp',
                          bd + 'Ordered/OrderedTaskBehaviour.cpp',
                          bd + 'Ordered/OrderedTask.cpp',
                          bd + 'Ordered/TaskAdvance.cpp',
                          bd + 'Ordered/TaskAdvanceSmart.cpp',
                          bd + 'Ordered/Points/IntermediatePoint.cpp',
                          bd + 'Ordered/Points/OrderedTaskPoint.cpp',
                          bd + 'Ordered/Points/StartPoint.cpp',
                          bd + 'Ordered/Points/FinishPoint.cpp',
                          bd + 'Ordered/Points/ASTPoint.cpp',
                          bd + 'Ordered/Points/AATPoint.cpp',
                          bd + 'Ordered/AATIsoline.cpp',
                          bd + 'Ordered/AATIsolineSegment.cpp',
                          bd + 'Unordered/UnorderedTask.cpp',
                          bd + 'Unordered/UnorderedTaskPoint.cpp',
                          bd + 'Unordered/GotoTask.cpp',
                          bd + 'Unordered/AbortTask.cpp',
                          bd + 'Unordered/AlternateTask.cpp',
                          bd + 'Factory/AbstractTaskFactory.cpp',
                          bd + 'Factory/RTTaskFactory.cpp',
                          bd + 'Factory/FAITaskFactory.cpp',
                          bd + 'Factory/FAITriangleTaskFactory.cpp',
                          bd + 'Factory/FAIORTaskFactory.cpp',
                          bd + 'Factory/FAIGoalTaskFactory.cpp',
                          bd + 'Factory/AATTaskFactory.cpp',
                          bd + 'Factory/MixedTaskFactory.cpp',
                          bd + 'Factory/TouringTaskFactory.cpp',
                          bd + 'Factory/Create.cpp',
                          bd + 'Points/TaskPoint.cpp',
                          bd + 'Points/SampledTaskPoint.cpp',
                          bd + 'Points/ScoredTaskPoint.cpp',
                          bd + 'Points/TaskLeg.cpp',
                          bd + 'ObservationZones/Boundary.cpp',
                          bd + 'ObservationZones/ObservationZoneClient.cpp',
                          bd + 'ObservationZones/ObservationZonePoint.cpp',
                          bd + 'ObservationZones/CylinderZone.cpp',
                          bd + 'ObservationZones/SectorZone.cpp',
                          bd + 'ObservationZones/LineSectorZone.cpp',
                          bd + 'ObservationZones/SymmetricSectorZone.cpp',
                          bd + 'ObservationZones/KeyholeZone.cpp',
                          bd + 'ObservationZones/AnnularSectorZone.cpp',
                          bd + 'ObservationZones/FAISectorZone.cpp',
                          bd + 'PathSolvers/TaskDijkstra.cpp',
                          bd + 'PathSolvers/TaskDijkstraMin.cpp',
                          bd + 'PathSolvers/TaskDijkstraMax.cpp',
                          bd + 'PathSolvers/IsolineCrossingFinder.cpp',
                          bd + 'Solvers/TaskMacCready.cpp',
                          bd + 'Solvers/TaskMacCreadyTravelled.cpp',
                          bd + 'Solvers/TaskMacCreadyRemaining.cpp',
                          bd + 'Solvers/TaskMacCreadyTotal.cpp',
                          bd + 'Solvers/TaskBestMc.cpp',
                          bd + 'Solvers/TaskSolveTravelled.cpp',
                          bd + 'Solvers/TaskCruiseEfficiency.cpp',
                          bd + 'Solvers/TaskEffectiveMacCready.cpp',
                          bd + 'Solvers/TaskMinTarget.cpp',
                          bd + 'Solvers/TaskOptTarget.cpp',
                          bd + 'Solvers/TaskGlideRequired.cpp',
                          bd + 'Solvers/TaskSolution.cpp',
                          bd + 'Stats/DistanceStat.cpp',
                          bd + 'Stats/CommonStats.cpp',
                          bd + 'Stats/ElementStat.cpp',
                          bd + 'Stats/TaskStats.cpp',
                          bd + 'Stats/TaskVario.cpp' ])

bd = topsrcdir + 'Terrain/'
e.StaticLibrary('terrain', [ bd + 'RasterBuffer.cpp',
                             bd + 'RasterProjection.cpp',
                             bd + 'RasterMap.cpp',
                             bd + 'RasterTile.cpp',
                             bd + 'RasterTileCache.cpp',
                             bd + 'Intersection.cpp',
                             bd + 'ScanLine.cpp',
                             bd + 'RasterTerrain.cpp',
                             bd + 'RasterWeather.cpp',
                             bd + 'HeightMatrix.cpp',
                             bd + 'RasterRenderer.cpp',
                             bd + 'TerrainRenderer.cpp',
                             bd + 'WeatherTerrainRenderer.cpp',
                             bd + 'TerrainSettings.cpp' ])

bd = topsrcdir + 'Thread/'
e.StaticLibrary('thread', [ topsrcdir + 'Poco/RWLock.cpp',
                            bd + 'Thread.cpp',
                            bd + 'SuspensibleThread.cpp',
                            bd + 'RecursivelySuspensibleThread.cpp',
                            bd + 'WorkerThread.cpp',
                            bd + 'StandbyThread.cpp',
                            bd + 'Mutex.cpp',
                            bd + 'Debug.cpp' ])

bd = topsrcdir + 'Engine/Waypoint/'
e.StaticLibrary('waypoint', [ bd + 'WaypointVisitor.cpp',
                              bd + 'Waypoints.cpp',
                              bd + 'Waypoint.cpp' ])

bd = topsrcdir + 'Widget/'
e.StaticLibrary('widget', [ bd + 'Widget.cpp',
                            bd + 'ActionWidget.cpp',
                            bd + 'WindowWidget.cpp',
                            bd + 'ListWidget.cpp',
                            bd + 'ContainerWidget.cpp',
                            bd + 'PanelWidget.cpp',
                            bd + 'TextWidget.cpp',
                            bd + 'LargeTextWidget.cpp',
                            bd + 'OverlappedWidget.cpp',
                            bd + 'TwoWidgets.cpp',
                            bd + 'RowFormWidget.cpp',
                            bd + 'ManagedWidget.cpp',
                            bd + 'PagerWidget.cpp',
                            bd + 'OffsetButtonsWidget.cpp',
                            bd + 'XMLWidget.cpp',
                            bd + 'ButtonPanelWidget.cpp',
                            bd + 'DockWindow.cpp' ])

