import os
import requests
import time
import logging
from datetime import datetime
from datetime import datetime, timezone
import zoneinfo

# Set up logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

# Set API Key and Channel ID from environment variables
API_KEY = os.getenv("YOUTUBE_API_KEY")
CHANNEL_ID = os.getenv("YOUTUBE_CHANNEL_ID")
LIKE_TRIGGER_URL = os.getenv("LIKE_TRIGGER_URL") # URL to trigger on new like
TIMEZONE = os.getenv("TIMEZONE")
START_TIME = int(os.getenv("START_TIME", 8))
END_TIME = int(os.getenv("END_TIME", 20))

# Polling intervals
VIDEO_FETCH_INTERVAL = 1800  # Fetch new videos every 30 min (1800 sec)
LIKE_CHECK_INTERVAL = 60  # Check likes every 1 min (60 sec)

# YouTube API URLs
SEARCH_URL = "https://www.googleapis.com/youtube/v3/search"
VIDEOS_URL = "https://www.googleapis.com/youtube/v3/videos"

# Store previous like counts and video IDs
previous_likes = {}
video_ids = []

def is_within_silent_hours():
    local_tz = zoneinfo.ZoneInfo(TIMEZONE)
    now = datetime.now().astimezone(local_tz)
    from datetime import timezone, timedelta
  # Set local timezone offset in hours
  # Convert to local time
    """Checks if the current time is between start and end time."""
    local_hour = now.hour
    return local_hour >= END_TIME or local_hour < START_TIME

def fetch_latest_video_ids():
    """Fetches the latest videos from the channel (up to 50)."""
    global video_ids
    params = {
        "part": "id",
        "channelId": CHANNEL_ID,
        "maxResults": 50,
        "type": "video",
        "order": "date",
        "key": API_KEY
    }
    response = requests.get(SEARCH_URL, params=params)
    data = response.json()

    video_ids = [item["id"]["videoId"] for item in data.get("items", [])]
    logging.info(f"✅ Fetched {len(video_ids)} latest video IDs.")

def get_video_likes():
    """Fetch like counts for stored video IDs."""
    if not video_ids:
        logging.warning("⚠️ No video IDs available. Skipping like check.")
        return {}

    video_data = {}
    for i in range(0, len(video_ids), 50):
        batch_ids = ",".join(video_ids[i:i+50])
        params = {
            "part": "snippet,statistics",
            "id": batch_ids,
            "key": API_KEY
        }
        response = requests.get(VIDEOS_URL, params=params)
        data = response.json()

        if "items" in data:
            for item in data["items"]:
                title = item["snippet"]["title"]
                like_count = int(item["statistics"].get("likeCount", 0))
                video_data[title] = like_count

    return video_data

def check_for_new_likes():
    """Checks if any video received new likes and triggers API request."""
    global previous_likes
    video_likes = get_video_likes()

    total_new_likes = 0
    
    for title, likes in video_likes.items():
        previous_likes.setdefault(title, likes)
        if likes > previous_likes[title]:
            total_new_likes += (likes - previous_likes[title])
        previous_likes[title] = likes
    
    if total_new_likes > 0 and not is_within_silent_hours():
        logging.info(f"🎉 {total_new_likes} new likes detected! Sending API request...")
        requests.get(LIKE_TRIGGER_URL)
    elif total_new_likes > 0:
        logging.info(f"⏳ {total_new_likes} new likes detected, but within silent hours. Skipping API request.")

def main():
    last_video_fetch_time = 0

    while True:
        current_time = time.time()
        is_within_silent_hours()
        # Fetch latest video IDs every 30 minutes
        if current_time - last_video_fetch_time > VIDEO_FETCH_INTERVAL:
            fetch_latest_video_ids()
            last_video_fetch_time = current_time

        # Check for new likes every minute
        check_for_new_likes()

        time.sleep(LIKE_CHECK_INTERVAL)

if __name__ == "__main__":
    main()
