/**
 * MediaController.java
 * @author doug
 *
 */
public class MediaController {
    
    /**
     * Set to true whenever something is modified, then false when checked.
     */
    private boolean updated = true;
    
    /**
     * List of recorded videos.
     */
    private MediaList media = new MediaList();
    
    /**
     * Title of the memo.
     */
    private String title = "";
    
    /**
     * Description of the memo (body).
     */
    private String description = "";
    
    /**
     * Sets the memo's title.
     * @param txt The string to set the memo to.
     */
    public void setTitle(String txt) {
        this.updated = true;
        this.title = txt;
    }
    
    /**
     * Sets the memo's description.
     * @param txt The string to set the memo to.
     */
    public void setDescription(String txt) {
        this.updated = true;
        this.description = txt;
    }

    /**
     * Returns the memo's title.
     * @return The memo's title.
     */
    public String getTitle() {
        this.updated = true;
        return this.title;
    }
    
    /**
     * Returns the memod's description.
     * @return The memo's description.
     */
    public String getDescription() {
        return this.description;
    }
    
    public void add(MediaItem item) {
        this.updated = true;
        this.media.add(item);
    }
    
    /**
     * Returns a video.
     * @param i The indeo of the video to return.
     * @return The video at index i.
     */
    public MediaItem get(int i) {
        return (Video)this.media.get(i);
    }
    
    /**
     * Returns a sound iterator.
     * @return A sound Iterator
     */
    public MediaNode iterator() {
        return this.media.iterator();
    }
    
    /**
     * Delete a media item.
     * @param index The index of the media item to delete.
     */
    public void delete(int index) {
        this.media.delete(index);
    }
    
    /**
     * Resets the class.
     * <ul>
     *  <li>Resets
     *   <ol>
     *    <li>Title</li>
     *    <li>Description</li>
     *      </ol>
     *  </li>
     *  <li>Deletes all
     *   <ol>
     *    <li>Videos</li>
     *    <li>Images</li>
     *    <li>Sounds</li>
     *   </ol>
     *  </li>
     * </ul>
     */
    public void reset() {
        this.updated = true;
        this.title = "";
        this.description = "";
        this.media.clear();
    }
    
    /**
     * Return number of recorded images. 
     * @return number of recorded images.
     */
    public int numItems() {
        return this.media.size();
    }
    
    public boolean updated() {
        boolean retVal = this.updated;
        this.updated = false;
        return retVal;
    }
} 

