function postFieldDataToOurServer ()
{
  var fdata = new FormData ();
  var request = new XMLHttpRequest ();
  var jresp;
  var inpField1 = document.getElementById ("inpField1");
  var inpField2 = document.getElementById ("inpField2");

  //populate the data that we are going to send
  fdata.append ("field1", inpField1.value);
  fdata.append ("field2", inpField2.value);
  fdata.append ("hiddenval", "some hidden value");


  //send the Data
//  request.open ("POST", "http://192.168.10.110/python2", false);
  request.open ("POST", "/python2", false);
  request.send (fdata);


  //receive and parse the output Data
  jresp = JSON.parse (request.responseText);
  console.log (jresp);

  if(document.getElementById("field").value == '')
  {
    alert("The textarea cannot be empty");
  }
  else if(jresp.english.length != 0){
    alert("The inputted text must be in English!");
  }
  else if(jresp.error.length !=0){
    alert("Please enter text with correct grammar and orthogrophy");
  }
}
