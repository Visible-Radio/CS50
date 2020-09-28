/* 
 

*/
function next_char(string_index, data_string, length) {
  if (data_string[string_index] != " ") {
    return string_index;
  } else {
    string_index++;
    next_char(string_index);
  }
}

write_to_tron("_ high voltage analog is better _");

function write_to_tron(data_string) {
  clear_tron();
  let l = data_string.length;
  if (l < 1) {
    return;
  }

  var row_heads = [0, 8, 16, 24];
  let string_index = 0;

  for (let i = 0; i < 32; i++) {
    
    
    let layer_group = document.getElementById(i);
    var layers = layer_group.querySelectorAll(".layer");

    if (row_heads.includes(i) && data_string[string_index] === " ") {
      // are we about to write a space at the head of a row?
      // then call our function to return the next string index where the char is not a space
      string_index = next_char(string_index + 1, data_string);
    }

    if (
      data_string[string_index] === " " ||
      data_string[string_index] === "." ||
      data_string[string_index] === "!"
    ) {
      document.getElementById(i).className = "off";
      layers[0].innerHTML = "~";
      layers[1].innerHTML = "~";
      layers[2].innerHTML = "~";
      layers[3].innerHTML = "~";
      string_index++;
    } else {
      document.getElementById(i).className = "on";
      layers[0].innerHTML = data_string[string_index];
      layers[1].innerHTML = data_string[string_index];
      layers[2].innerHTML = data_string[string_index];
      layers[3].innerHTML = data_string[string_index];
      string_index++;
      
    }
    if (string_index >= l) {
        break;
      }
  }
}

function clear_tron() {
  for (let i = 0; i < 32; i++) {
    let layer_group = document.getElementById(i);
    var layers = layer_group.querySelectorAll(".layer");
    document.getElementById(i).className = "off";    
  }
}

function sleep(milliseconds) {
  const date = Date.now();
  let currentDate = null;
  do {
    currentDate = Date.now();
  } while (currentDate - date < milliseconds);
}