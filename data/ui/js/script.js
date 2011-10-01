google.load("jquery", "1.6.4");
google.load("jqueryui", "1.8.16");
google.load("gdata", "1");

var tilePaneHeight = 300;
var tileMargin = 1;
var tilePanePadding = 1;

google.setOnLoadCallback(function(){
$(document).ready(function(){



// TilePane
function TilePane(tw, th) {
	this.tiles = new Array();
	
	this.tileWidth = tw;
	this.tileHeight = th;
	
	this.margin = tileMargin;
	this.padding = tilePanePadding;
	
	this.totalHeight = parseInt($("#tile-pane-holder").height() - this.padding*2);
	
	// set the tile pane's position
	var pos = $("#tile-pane-holder").offset();
	$("#tile-pane").css({"left": pos.left+"px", "top": pos.top+"px"});
	$("#tile-pane").css("padding", this.padding+"px");
}

TilePane.prototype.push = function(tile) {
    this.tiles.push(tile);
	this.update();
};

TilePane.prototype.setTiles = function(tileArr) {
	this.tiles = tileArr;
	this.update();
};

TilePane.prototype.clear = function() {
	this.tiles = new Array();
	this.update();
};

TilePane.prototype.update = function() {
	var availableWidth = parseInt($("#tile-pane-holder").width()) - this.padding*2;
	
	// set the masked pane width and height
	$("#tile-pane").width(availableWidth);
	$("#tile-pane").height($("#tile-pane-holder").height() - this.padding*2);
	
	// set the total height
	var numOfColumns = parseInt(availableWidth/(this.tileWidth+this.margin*2));
	this.totalHeight = Math.ceil(this.tiles.length/numOfColumns)*(this.tileWidth + this.margin*2);
	var minHeight = parseInt($("#tile-pane-holder").height() - this.padding*2);
	if(this.totalHeight < minHeight){
		this.totalHeight = minHeight;
	}
};

TilePane.prototype.setPadding = function(value) {
	this.padding = value;
	this.update();
};

TilePane.prototype.setMargin = function(value) {
	this.margin = value;
	this.update();
};


// tile pane class
function Tile(w, h) {
	this.width = w;
	this.height = h;
	
	var tiles = new Array(10);
	
	var jCanvas = jQuery("<canvas/>");
	jCanvas.attr("width", this.width);
	jCanvas.attr("height", this.height);
	jCanvas.css("margin", tileMargin+"px");
	jCanvas.css("float", "left");
	$("#tile-pane").append(jCanvas);
	
	if(jCanvas[0].getContext){
		var ctx = jCanvas[0].getContext('2d');
		
		ctx.fillStyle = "rgb(255, 0, 0)";
		ctx.fillRect(0,0,this.width, this.height);
	}
	else{
		console.log("can not get context!");
	}
	
	// bind the click event handler
	jCanvas.bind("click", function(){
		console.log("tile " + $(this).offset().left);
	});
	
	console.log("height: " + $("#tile-pane").css("width"));
}

var tilePane = new TilePane(48, 48);
$("#tile-pane").hover(
	// mouse over
	function(){
		$(this).clearQueue();
		$(this).animate({
		height: tilePane.totalHeight
		}, 100);
	},
	// mouse out
	function(){
		$(this).clearQueue();
		$(this).animate({
		height: 86 - tilePane.padding*2
		}, 100);
	}
);


// create tiles and tile pane
var tileArr = new Array();
for(var i=0; i<100; ++i){
	var tile = new Tile(48, 48);
	tileArr.push(tile);
}
tilePane.setTiles(tileArr);

// window resize update tile pane
$(window).resize(function(e){
	tilePane.update();
});


	
});// on document ready
});// google onload function