present = new Array();
present[0] = 1;
present[1] = 1;
window.cacheMenuDistance = new Array();
window.currentLocation = "Header";

$(document).ready(function() {

	splashLoading();

	var menu = $(".menu-items").children("li");
	menu.each(function(i) {
		window.cacheMenuDistance[i] = {
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

		if (scrollLength + $(window).height() - $(window).height()/5 >= offsets[2] && present[0]) {
			$("#uiuc-book").addClass("slideLeftToRight");
			$("#uiuc-book").css("display", "initial");
			present[0] = 0;
		}

		if (scrollLength + $(window).height() - $(window).height()/5 >= offsets[4] && present[1]) {
			$(".contactHeader").addClass("slideLeftToRight");
			$(".contactHeader").css("display", "initial");
			present[1] = 0;
		}

		if (scrollLength <= offsets[1] - 150.0 && window.currentLocation != "Header") {
			movingPanel.animate({
				"margin-left": window.cacheMenuDistance[0].offset,
				"width": window.cacheMenuDistance[0].length
			}, 200);
			window.currentLocation = "Header";
		}

		if (scrollLength >= offsets[1] - 150.0 && scrollLength <= offsets[2] - 150.0 && window.currentLocation != "aboutMe") {
			movingPanel.animate({
				"margin-left": window.cacheMenuDistance[1].offset,
				"width": window.cacheMenuDistance[1].length
			}, 200);
			window.currentLocation = "aboutMe";
		}

		if (scrollLength >= offsets[2] - 150.0 && scrollLength <= offsets[3] - 150.0 && window.currentLocation != "education") {
			movingPanel.animate({
				"margin-left": window.cacheMenuDistance[2].offset,
				"width": window.cacheMenuDistance[2].length
			}, 200);
			window.currentLocation = "education";
		}

		if (scrollLength >= offsets[3] - 150.0 && scrollLength <= offsets[4] - 150.0 && window.currentLocation != "projects") {
			movingPanel.animate({
				"margin-left": window.cacheMenuDistance[3].offset,
				"width": window.cacheMenuDistance[3].length
			}, 200);
			window.currentLocation = "projects";
		}

		if (scrollLength >= offsets[4] - 150.0 && window.currentLocation != "contact") {
			movingPanel.animate({
				"margin-left": window.cacheMenuDistance[4].offset,
				"width": window.cacheMenuDistance[4].length
			}, 200);
			window.currentLocation = "contact"
		}


		if (scrollLength > offsets[1]) {
			return;
		}


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

	var temp = $(".active");
	temp.animate({
				"margin-left": window.cacheMenuDistance[0].offset,
				"width": window.cacheMenuDistance[0].length
			}, 0);


	var buttons = $(".navi-bar").find("ul").children("li");
	var sectionScrollPosition = new Array();

	buttons.each(function(i) {
		var idOf = $(this).attr("id");
		idOf += "Section";

		var offset = ($("#" + idOf).offset().top - $(".navi-bar").height()) ;
		sectionScrollPosition[i] = offset;
	});

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