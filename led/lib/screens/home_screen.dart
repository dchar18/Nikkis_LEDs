import 'dart:ui';
import 'package:http/http.dart';

import "package:flutter/material.dart";
import 'package:led/widgets/mode_list.dart';
import 'package:led/widgets/preset_colors.dart';
import 'package:led/widgets/slider_text.dart';
import 'package:shared_preferences/shared_preferences.dart';

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

  List<Color> presets = [
    Color(0x9ffb0022), // pink (155,0,34)
    Color(0xfffc0a00), // blood red (252,10,0)
    Color(0xffff0000),
    Color(0xffff0022), // hot pink (255,0,34)
    Color(0xffff2b00), // orange (255,43,0)
    Color(0xffff2b0a), // peach (255,43,10)
    Color(0xff50ff41), // mint green (80,255,65)
    Color(0xff00ff00),
    Color(0xff00ffff), // aqua blue (0,255,255)
    Color(0xff0fd0ff), // light blue (15,208,255)
    Color(0xff0000ff),
    Color(0xff1560ff), // metal blue (21,96,255)
    Color(0xffff00ff), // purple (255,0,255)
  ];

  @override
  void initState() {
    super.initState();
    _loadRGB();
  }

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
            // SizedBox(height: 30),
            SizedBox(height: 45),
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
            Container(
              height: 60,
              width: MediaQuery.of(context).size.width,
              padding: EdgeInsets.all(10),
              child: ListView.builder(
                scrollDirection: Axis.horizontal,
                shrinkWrap: true,
                itemCount: presets.length,
                itemBuilder: (BuildContext context, int index) {
                  final Color color = presets[index];
                  return GestureDetector(
                    onTap: () {
                      setState(() {
                        _redValue = color.red.toDouble();
                        _greenValue = color.green.toDouble();
                        _blueValue = color.blue.toDouble();
                        updateRGB();
                      });
                    },
                    child: Container(
                      padding: EdgeInsets.all(10),
                      margin: EdgeInsets.only(left: 5, right: 5),
                      height: 40,
                      width: 40,
                      decoration: BoxDecoration(
                          color: color,
                          borderRadius: BorderRadius.all(Radius.circular(20)),
                          border: Border.all(
                              width: 4,
                              color: Colors.white,
                              style: BorderStyle.solid)),
                    ),
                  );
                },
              ),
            ),
            Expanded(
              child: modeList(),
            ),
          ],
        ),
      ),
    );
  }

  _loadRGB() async {
    SharedPreferences pref = await SharedPreferences.getInstance();
    setState(() {
      _redValue = pref.get('red') ?? 0.0;
      _greenValue = pref.get('green') ?? 0.0;
      _blueValue = pref.get('blue') ?? 0.0;
    });
  }

  updateRGB() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    setState(() {
      prefs.setDouble('red', _redValue);
      prefs.setDouble('green', _greenValue);
      prefs.setDouble('blue', _blueValue);
    });
    sendRGB();
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
  print("Entering mode: " + mode);
  String url;
  mode = mode.toLowerCase();
  if (mode.contains("twinkle")) {
    mode = mode.replaceAll(" twinkle", "");
    print("current mode: " + mode);
    mode = "twinkle_" + mode;
  } else if (mode.contains(" ")) {
    mode = mode.replaceAll(" ", "_");
  }

  url = "http://192.168.50.70/" + mode.toLowerCase();

  print("Using url: " + url);
  final response = await get(url);
  // print(response.body);
  print("Response: " + response.statusCode.toString());
}
