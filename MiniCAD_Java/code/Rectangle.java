public final class Rectangle implements DrawableShape {

    ////////////////////////
    // Variables

    private final Point upperLeftCoord;
    private final int width;
    private final int height;
    private final int outlineColor;
    private final int fillColor;

    // End variables
    ////////////////////////



    ////////////////////////
    // Methods

    public Point getUpperLeftCoord() {
        return upperLeftCoord;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

    public int getOutlineColor() {
        return outlineColor;
    }

    public int getFillColor() {
        return fillColor;
    }

    public Rectangle(final int x, final int y, final int width, final int height,
                     final int outlineColor, final int fillColor) {
        upperLeftCoord = new Point(x, y);
        this.width = width;
        this.height = height;
        this.outlineColor = outlineColor;
        this.fillColor = fillColor;
    }

    public void getDrawn(final Painter canvas) {
        canvas.draw(this);
    }

    // End methods
    ////////////////////////

}
