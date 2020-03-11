// Put all onload AJAX calls here, and event listeners
$(document).ready(function () {
    // On page-load AJAX Example
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/someendpoint',   //The server endpoint we are connecting to
        data: {
            name1: "Value 1",
            name2: "Value 2"
        },
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            $('#blah').html("On page load, received string '" + data.foo + "' from server");
            //We write the object to the console to show that the request was successful
            console.log(data);

        },
        fail: function (error) {
            // Non-200 return, do something with error
            $('#blah').html("On page load, received error from server");
            console.log(error);
        }
    });

    //hides and shows the content table div
    $('.tab.file-log').click(function (e) {
        if ($('.file-log-wrapper').css("display") === "none") {
            $('.file-log-wrapper').css("display", "block");
        } else {
            $('.file-log-wrapper').css("display", "none");
        }

        e.preventDefault(); //No redirects if possible
    });

    $('.tab.details').click(function (e) {
        if ($('.detail-wrapper').css("display") === "none") {
            $('.detail-wrapper').css("display", "block");
        } else {
            $('.detail-wrapper').css("display", "none");
        }

        e.preventDefault(); //No redirects if possible
    });

});