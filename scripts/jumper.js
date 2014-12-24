present = new Array();
present[0] = 1;

$(document).ready(function() {

	splashLoading();

	var sectionScrollPosition = setupJumpTo();
	setupParallaxForHeader(sectionScrollPosition);




});

function setupParallaxForHeader(offsets) {

	$(window).scroll(function() {
		if ($(window).scrollTop() + $(window).height() - $(window).height()/5 >= offsets[2] && present[0]) {
			$("#uiuc-book").addClass("slideLeftToRight");
			$("#uiuc-book").css("display", "initial");
			present[0] = 0;
		}

		if ($(window).scrollTop() > offsets[1]) {
			return;
		}


		var temp = $("#main-intro");
		var background = $("#brandingSection");
		temp.css("margin-top", $(window).scrollTop());
		background.css("background-position-y", -$(window).scrollTop() * .5);
		temp.css("opacity", 1*Math.exp(-$(window).scrollTop() * 0.005));
	});

}

function splashLoading() {







}

function setupJumpTo() {

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