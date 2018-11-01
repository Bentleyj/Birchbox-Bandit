var pm2 = require('pm2');
var express = require('express');
var app = express();
var schedule = require('node-schedule');
var nodeCleanup = require('node-cleanup');
var moment = require('moment');
var minimist = require('minimist');

var appName = "BrichboxBandit"

// Print the parameters you may pass to this app.
printUsage();

// Connect to an existing PM2 instance or create one if none exists.
pm2.connect(function(err) {
	// If there is an error here, abort.
	if(err) {
		console.log(err)
		process.exit(2);
	}

	// Retrieve command line options
	var opts = minimist(process.argv.slice(2));

	// Check for Startup command line option to determine app launch time.
	if(undefined != opts['app-launch-hour']) {
		var launchDate = moment().hour(opts['app-launch-hour']);
		if(launchDate.isAfter(moment())) {
			scheduleStartup(launchDate);
		} else {
			console.log("Specified startup date is in the past, launching app immediately.");
			launchApp();
		}
	} else {
		console.log("No app Launch time specified, launching app immediately.");
		launchApp();
	}

	// Check for Shutdown command line option to determine app close time.
	if(undefined != opts['app-close-hour']) {
		var closeDate = moment().hour(opts['app-close-hour']);
		if(closeDate.isAfter(moment())) {
			scheduleShutdown(closeDate);
		} else {
			console.log("Specified close date is in the past, closing app in 20 seconds.");
			scheduleShutdown(moment().add(20, 's').toDate())
		}
	} else {
		console.log("No app close time specified, app will close when the server is killed.");
	}

	// Listen on port 1555
	app.listen(1555, function() {
		console.log('Listening on port 1555!');
	});
});

// Setup cleanup so PM2 closes app when this process is also closed.
nodeCleanup(function (exitCode, signal) {
	if(signal) {
		console.log(signal);
		pm2.delete(appName, function(err) {
			console.log(appName + " closed and removed from PM2.");
		});
		nodeCleanup.uninstall();
		return false;
	}
});

/* --------------------------------------------------------------------------- */

function scheduleShutdown(date) {
	var shutdownJob = schedule.scheduleJob(date, closeApp);
	console.log('Shutdown app scheduled for:' + moment(date).format('HH:MM:SS DD/MM/YYYY'));
	return shutdownJob;
}

/* --------------------------------------------------------------------------- */

function scheduleStartup(date) {
	var shutdownJob = schedule.scheduleJob(date, launchApp);
	console.log('Startup app scheduled for:' + moment(date).format('HH:MM:SS DD/MM/YYYY'));
	return shutdownJob;
}

/* --------------------------------------------------------------------------- */

function launchApp() {
	pm2.start({
		name: appName,
		script: "test_sliders/bin/test_sliders_debug.exe",
		exec_mode: "fork",
		instances: "1",
		interpreter: "none",
		maxRestarts: "3"
	}, function(err, proc) {
		if(err) {
			throw err;
		} else {
			// console.log(proc);
		}
		pm2.disconnect();
	});
}

/* --------------------------------------------------------------------------- */

function closeApp() {
		pm2.delete(appName, function(err) {
			console.log(appName + " closed and removed from PM2.");
		});
}

/* --------------------------------------------------------------------------- */

function printUsage() {
  var app = process.argv[1];
  console.log("Welcome to the app scheduling controller!")
  console.log("");
  console.log("");
  console.log("Usage: ");
  console.log("--app-launch-hour: Manually set an hour that the app will launch between 0 and 23. If no hour is specificed the app will launch immediately. If the hour has passed the app will launch immediately.");
  console.log("--app-close-hour: Manually set an hour that the app will close between 0 and 23. If no hour is specified the app will close when the server is closed. If the hour has passed the app will close after a 20 second delay.");
  console.log("");
  console.log("");
};