$(document).ready(function() {

	var heightOfPage = $(window).height();

	var menuItems = $(".menu-item");

	for (var i = 0; i < menuItems.length; i++) {

		menuItems[i].onclick = function() {
			var idOf = this.id;
			console.log(idOf);
			switch (idOf) {
				case "aboutButton":
					$('html, body').animate({
				        scrollTop: 0
				    }, 1000);
					break;
				case "wordButton":
					$('html, body').animate({
				        scrollTop: $("#personalprojects").offset().top
				    }, 1000);
					break;
				case "samplesButton":
					$('html, body').animate({
				        scrollTop: $("#codesamples").offset().top
				    }, 1000);
					break;
				default: return;
			}

		};

	}

});