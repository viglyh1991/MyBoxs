搭建 Mapzen Turn-by-Turn

https://mapzen.com/documentation/mobility/turn-by-turn/add-routing-to-a-map/


1.Sign up for a Mapzen Turn-by-Turn API key https://mapzen.com/developers 
2.Create an index page
3.Start a server to get the project running locally
  > python -m SimpleHTTPServer
  > http://localhost:8000
4. Add references to CSS and JavaScript files
  >The Leaflet JavaScript library, which provides tools for zooming, displaying attributions, and drawing symbols, is one way you can display routes on web and mobile maps. 
  > add a reference to the Leaflet CSS file.
  > add a reference to the Mapzen CSS file.
  > add the Leaflet JavaScript file.
  > Add the Tangram JavaScript file
  > Add the Leaflet Routing Machine JavaScript file.
  > Add the Mapzen JavaScript file.
5.Add a map to the page
6.Add a Tangram map to the frame
7.Add waypoints for routing
8.Set Mapzen Turn-by-Turn as the routing engine
9.Change the route line color
10.Walkthrough summary and next steps
