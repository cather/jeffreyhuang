$(document).ready(main);

function main()
{
    var boxes = $(".box");
    $(boxes).each(function() {
        var children = $(this).children();
        var left = $(children).eq(0), right = $(children).eq(1);
        $(left).height($(left).height() > $(right).height() ? $(left).height() : $(right).height());
        $(right).height($(left).height() > $(right).height() ? $(left).height() : $(right).height());
    });
    
    var workInfo = parseWorkInformation();
    

    var otherInfo = parseOtherInformation();
    
    
    $('.other-injection-point').html(otherInfo);
    $('.work-injection-point').html(workInfo);

}

function parseWorkInformation()
{
    var work_info = $('.work-information');
    var html = "";
    
    for (var i = 0; i < work_info.length; i++)
    {
        html += parseWorkBlurb(work_info[i]);
    }
    
    $(work_info).each(function() { $(this).remove(); });
    
    return html;
}

function parseWorkBlurb(info)
{
    var name = $(info).attr('data-name');
    var title = $(info).attr('data-title');
    var start = $(info).attr('data-start');
    var end = $(info).attr('data-end');
    var png = $(info).attr('data-png');
    var brag = $(info).html();
    
    var html = "<li class='work-information-parsed row'>";
    html    +=      "<p class='col-md-9 work-info'>";
    html    +=          "<span class='work-info-employer'>" + name + "</span>";
    html    +=          "<br/>";
    html    +=          "<span class='work-info-title'>" + title + "</span>";
    html    +=          "<br/>";
    html    +=          "<span class='work-info-time'>" + start + " ~ " + end + "</span>";
    html    +=          "<br/><br/>";
    html    +=          "<span class='work-info-brag'>" + brag + "</span>";
    html    +=      "</p>";
    html    +=      "<img src='" + 'images/' + png + "' class='work-info-png col-md-3' \>";
    html    += "</li>"
    
    return html;
    
}

function parseOtherInformation()
{
    var other = $('.other-information');
    var html = "";
    
    for (var i = 0; i < other.length; i++)
    {
        html += parseOtherBlurb(other[i]);
    }
    
    $(other).each(function() { $(this).remove(); });
    
    return html;
}

function parseOtherBlurb(info)
{
    var name = $(info).attr('data-name');
    var title = $(info).attr('data-title');
    var done = $(info).attr('data-done');
    var partners = $(info).attr('data-partners');
    var png = $(info).attr('data-png');
    var brag = $(info).html();
    
    var html = "<li class='other-information-parsed row'>";
    html    +=      "<p class='col-md-9 other-info'>";
    html    +=          "<span class='other-info-name'>" + name + "</span>";
    html    +=          "<br/>";
    html    +=          "<span class='other-info-title'>" + title + "</span>";
    html    +=          "<br/>";    
    html    +=          "<span class='other-info-teams'>" + partners + "</span>";
    html    +=          "<br/>";
    html    +=          "<span class='other-info-day'>" + done + "</span>";
    html    +=          "<br/><br/>";
    html    +=          "<span class='other-info-brag'>" + brag + "</span>";
    html    +=      "</p>";
    html    +=      "<img src='" + 'images/' + png + "' class='other-info-png col-md-3' \>";
    html    += "</li>"
    
    return html;
}