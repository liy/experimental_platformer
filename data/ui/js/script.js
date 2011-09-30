google.load("jquery", "1.6.4");
google.load("jqueryui", "1.8.16");
google.load("gdata", "1");

google.setOnLoadCallback(function(){

$(document).ready(function(){
	function setMapPanePos(){
		var pos = $("#map-pane-holder").offset();
		$("#map-pane").css({"left": pos.left+"px", "top": pos.top+"px"});
		$("#map-pane").width($("#map-pane-holder").width());
		$("#map-pane").height($("#map-pane-holder").height());
	}
	setMapPanePos();
	
	$(window).resize(function(e){
		setMapPanePos();
	});
	
	
	$("#map-pane").hover(
		// mouse over
		function(){
			$(this).clearQueue();
			$(this).animate({
			height: 300
			}, 500);
		},
		// mouse out
		function(){
			$(this).clearQueue();
			$(this).animate({
			height: 86
			}, 500);
		}
	);
});

// map pane class
function TilePane(w, h) {
	this.width = w;
	this.height = h;
	
	var tiles = new Array(10);
}

TilePane.prototype.clear = function(){
    
};
	
TilePane.prototype.addTile = function(){
};
	
TilePane.prototype.getTile = function(){
};

TilePane.prototype.resize = function(w, h){
	this.width = w;
	this.height = h;
	console.log("width: " + this.width);
	console.log("height: " + this.height);
};

TilePane.prototype.onMouseClick = function(){
};

var pane1 = new TilePane(100, 100);
pane1.resize(200, 200);

var pane2 = new TilePane(300, 300);
pane2.resize(400, 400);



});// google onload function