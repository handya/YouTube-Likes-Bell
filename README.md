# YouTube-Bell

This script tracks new likes and subscriber counts for a YouTube channel using the YouTube Data API. When a new like or subscriber is detected, it triggers API requests to specified URLs.

## Features
- Fetches the latest video IDs from a YouTube channel.
- Monitors video likes and detects increases.
- Tracks subscriber count changes.
- Triggers external API calls when new likes or subscribers are detected.
- Configurable silent hours to prevent triggering notifications at night.
- Includes error handling for API request failures to `googleapis.com`.
- Supports running in Docker for easy deployment.

## Requirements
- Python 3
- `requests` library
- A YouTube Data API key

## Installation
1. Clone this repository:
   ```sh
   git clone https://github.com/handya/YouTube-Likes-Bell.git
   cd YouTube-Likes-Bell
   ```
2. Install dependencies:
   ```sh
   pip install requests
   ```
3. Set up environment variables:
   ```sh
   export YOUTUBE_API_KEY="your_api_key"
   export YOUTUBE_CHANNEL_ID="your_channel_id"
   export LIKE_TRIGGER_URL="your_like_trigger_url"
   export SUBSCRIBER_TRIGGER_URL="your_subscriber_trigger_url"
   export TIMEZONE="Pacific/Auckland"
   export START_TIME=8
   export END_TIME=20
   ```

## Usage
Run the script with:
```sh
python script.py
```

## Running with Docker
This script supports Docker for easier deployment.

### Build the Docker Image
```sh
docker build -t YouTube-Likes-Bell .
```

### Run the Container
```sh
docker run -d --env YOUTUBE_API_KEY=your_api_key \
    --env YOUTUBE_CHANNEL_ID=your_channel_id \
    --env LIKE_TRIGGER_URL=your_like_trigger_url \
    --env SUBSCRIBER_TRIGGER_URL=your_subscriber_trigger_url \
    --env TIMEZONE=Pacific/Auckland \
    --env START_TIME=8 \
    --env END_TIME=20 \
    YouTube-Likes-Bell
```

### Stopping the Container
To stop the running container:
```sh
docker ps  # Find the container ID

docker stop <container_id>
```

## Configuration
The following environment variables can be configured:
- `YOUTUBE_API_KEY`: Your YouTube Data API key.
- `YOUTUBE_CHANNEL_ID`: The ID of the YouTube channel to track.
- `LIKE_TRIGGER_URL`: API endpoint to trigger when a new like is detected.
- `SUBSCRIBER_TRIGGER_URL`: API endpoint to trigger when a new subscriber is detected.
- `TIMEZONE`: The timezone for checking silent hours (default: `Pacific/Auckland`).
- `START_TIME`: The start of active hours (default: `8`, i.e., 8 AM).
- `END_TIME`: The end of active hours (default: `20`, i.e., 8 PM).

## Arduino Integration
This project includes an Arduino-based API server that listens for requests from the Python script. When a request is received, the Arduino triggers an event (e.g., playing a bell sound).

### Requirements
- Arduino with Ethernet support
- Ethernet Shield
- Miniature Solenoid 

### Setup
1. Flash the provided Arduino sketch onto your device.
2. Ensure your Arduino is connected to the network with the specified IP address.
3. The Python script will send HTTP requests to the Arduino API to trigger events.

### API Commands
The Arduino listens for the following commands:
- `playonce` - Triggers a single short beep.
- `playtwice` - Triggers two short beeps.
- `playthrice` - Triggers three short beeps.

## Error Handling
- The script includes error handling for requests to `googleapis.com`, ensuring it logs errors instead of crashing.
- If an API request fails, the script logs the error and continues running.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.


