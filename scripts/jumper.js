present = new Array();
present[0] = 1;
present[1] = 1;
window.cacheMenuDistance = new Array();
window.currentLocation = "Header";

$(document).ready(function() {

	splashLoading();

	// grab the menu buttons and stores into menu
	var menu = $(".menu-items").children("li");

	// runs the loop that gets the offsets for each menu button
	menu.each(function(i) {
		window.cacheMenuDistance[i] = {		// grabs the memu distance length and offset so you can make the moving bar move around
			offset: $(this).offset().left,
			length: $(this).width(),
		};
	});
	var sectionScrollPosition = setupJumpTo();
	



	setupParallaxForHeader(sectionScrollPosition);




});

function setupParallaxForHeader(offsets) {

	$(window).scroll(function() {
		var scrollLength = $(window).scrollTop();	// cache the scroll offset from the top
		var cacheMenuDistance;
		var movingPanel = $(".active");

		if (scrollLength + $(window).height() - $(window).height()/5 >= offsets[2] && present[0]) {	// if the distance from the top is greater than the 2nd offset present the book with a fade in effect
			$("#uiuc-book").addClass("slideLeftToRight");
			$("#uiuc-book").css("display", "initial");
			present[0] = 0;
		}

		// If it scrolled past the projects page fade in the contact me text
		if (scrollLength + $(window).height() - $(window).height()/5 >= offsets[4] && present[1]) {
			$(".contactHeader").addClass("slideLeftToRight");
			$(".contactHeader").css("display", "initial");
			present[1] = 0;
		}

		// all of the following is logic for the bar that moves around in the menu bar
		if (scrollLength <= offsets[1] - 150.0 && window.currentLocation != "Header") {
			movingPanel.animate({
				"margin-left": window.cacheMenuDistance[0].offset,
				"width": window.cacheMenuDistance[0].length
			}, 200);
			window.currentLocation = "Header";
		}

		// for the about me part
		if (scrollLength >= offsets[1] - 150.0 && scrollLength <= offsets[2] - 150.0 && window.currentLocation != "aboutMe") {
			movingPanel.animate({
				"margin-left": window.cacheMenuDistance[1].offset,
				"width": window.cacheMenuDistance[1].length
			}, 200);
			window.currentLocation = "aboutMe";
		}

		// for the education part
		if (scrollLength >= offsets[2] - 150.0 && scrollLength <= offsets[3] - 150.0 && window.currentLocation != "education") {
			movingPanel.animate({
				"margin-left": window.cacheMenuDistance[2].offset,
				"width": window.cacheMenuDistance[2].length
			}, 200);
			window.currentLocation = "education";
		}

		// for the projects part
		if (scrollLength >= offsets[3] - 150.0 && scrollLength <= offsets[4] - 150.0 && window.currentLocation != "projects") {
			movingPanel.animate({
				"margin-left": window.cacheMenuDistance[3].offset,
				"width": window.cacheMenuDistance[3].length
			}, 200);
			window.currentLocation = "projects";
		}

		// for the contact me part
		if (scrollLength >= offsets[4] - 150.0 && window.currentLocation != "contact") {
			movingPanel.animate({
				"margin-left": window.cacheMenuDistance[4].offset,
				"width": window.cacheMenuDistance[4].length
			}, 200);
			window.currentLocation = "contact"
		}

		// this is for parallax
		if (scrollLength > offsets[1]) {
			return;
		}

		// parallax effect where the text moves at a different pace than the background
		var temp = $("#main-intro");
		var background = $("#brandingSection");
		temp.css("margin-top", scrollLength);
		background.css("background-position-y", -scrollLength * .5);
		temp.css("opacity", 1*Math.exp(-$(window).scrollTop() * 0.005));
	});

}

function splashLoading() {







}

function setupJumpTo() {

	// initial loading of the website
	var temp = $(".active");
	temp.animate({
				"margin-left": window.cacheMenuDistance[0].offset,
				"width": window.cacheMenuDistance[0].length
			}, 0);


	var buttons = $(".navi-bar").find("ul").children("li");
	var sectionScrollPosition = new Array();

	// run a loop on each of the buttons
	buttons.each(function(i) {
		// grab the id of the section that corresponds to the menu item
		var idOf = $(this).attr("id");
		idOf += "Section";

		// grab the offset of each section
		var offset = ($("#" + idOf).offset().top - $(".navi-bar").height()) ;
		sectionScrollPosition[i] = offset;
	});

	// create the event handlers for each of the menu buttons
	buttons.each(function(i) {
		$(this).on("click", function() {
			$("html, body").animate({
				scrollTop: sectionScrollPosition[i],
				duration: 400,
			});
		});
	});

	return sectionScrollPosition;


}