import 'dart:ui';
import 'package:http/http.dart';

import "package:flutter/material.dart";
import 'package:led/widgets/mode_list.dart';
import 'package:led/widgets/preset_colors.dart';
import 'package:led/widgets/slider_text.dart';

class HomeScreen extends StatefulWidget {
  @override
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  double _redValue = 0;
  double _greenValue = 0;
  double _blueValue = 0;

  Color red = Color(0xffef5350);
  Color green = Color(0xff9ccc65);
  // Color green = Color(0xff81c784);
  // Color blue = Color(0xff64b5f6);
  Color blue = Color(0xff4fc3f7);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Container(
        height: MediaQuery.of(context).size.height,
        width: MediaQuery.of(context).size.width,
        decoration: BoxDecoration(
          gradient: LinearGradient(
            begin: Alignment.topLeft,
            end: Alignment(
                0.8, 0.0), // 10% of the width, so there are ten blinds.,
            colors: [const Color(0xffc7a4ff), const Color(0xff9575cd)],
            // colors: [const Color(0xff03a9f4), const Color(0xff007ac1)],
          ),
        ),
        child: Column(
          children: [
            SizedBox(height: 30),
            Container(
              height: 75,
              width: MediaQuery.of(context).size.width,
              padding:
                  EdgeInsets.only(top: 10, bottom: 10, left: 15, right: 10),
              child: Text(
                "LED Control",
                style: TextStyle(
                    fontSize: 35,
                    fontWeight: FontWeight.normal,
                    fontFamily: 'Roboto'),
              ),
              alignment: Alignment.centerLeft,
            ),
            sliderText("Red", _redValue, red),
            Slider(
              value: _redValue,
              min: 0,
              max: 255,
              label: _redValue.round().toString(),
              onChanged: (double value) {
                setState(() {
                  _redValue = value;
                });
              },
              onChangeEnd: (double value) => sendRGB(),
              activeColor: red,
              inactiveColor: Colors.red[300],
            ),
            SizedBox(height: 5),
            sliderText("Green", _greenValue, green),
            Slider(
              value: _greenValue,
              min: 0,
              max: 255,
              label: _greenValue.round().toString(),
              onChanged: (double value) {
                setState(() {
                  _greenValue = value;
                });
              },
              onChangeEnd: (double value) => sendRGB(),
              activeColor: green,
              inactiveColor: Colors.green[300],
            ),
            SizedBox(height: 5),
            sliderText("Blue", _blueValue, blue),
            Slider(
              value: _blueValue,
              min: 0,
              max: 255,
              label: _blueValue.round().toString(),
              onChanged: (double value) {
                setState(() {
                  _blueValue = value;
                });
              },
              onChangeEnd: (double value) => sendRGB(),
              activeColor: blue,
              inactiveColor: Colors.blue[300],
            ),
            presetColors(context),
            Expanded(
              child: modeList(),
            ),
          ],
        ),
      ),
    );
  }

  // this function sends solid rgb values to a specific board/all boards
  sendRGB() async {
    String combination = _redValue.round().toString() +
        "/" +
        _greenValue.round().toString() +
        "/" +
        _blueValue.round().toString();
    String address = "http://192.168.50.70/rgb/" + combination;

    print("Sending to " + address);

    final response = await get(address);
    // print(response.body);
    print("Response code: " + response.statusCode.toString());
  }
}

// this function is used to send a mode (not solid rgb) to a specific board or to all
void sendRequest(String mode) async {
  // print("Received " + device + ", entering mode: " + mode);
  // String url;
  // if (mode.contains("twinkle")) {
  //   mode = mode.replaceAll(" twinkle", "");
  //   print("current mode: " + mode);
  //   mode = "twinkle_" + mode;
  // }
  // if (device == "All") {
  //   url = "http://192.168.50.114:8181/" + mode.toLowerCase() + "/sync";
  // } else {
  //   url = "http://192.168.50.114:8181/esp8266_" +
  //       device.toLowerCase() +
  //       "/" +
  //       mode.toLowerCase();
  // }

  // print("Using url: " + url);
  // final response = await get(url);
  // // print(response.body);
  // print("Response: " + response.statusCode.toString());
}
