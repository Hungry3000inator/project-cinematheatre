#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "MyForm.h"

using namespace std;

class CinemaBookingTest : public ::testing::Test {
protected:
    CinemaBooking booking;
    FilmList filmList;
    SessionList sessionList;
    SeatList seatList;
    UserAuthService authService;
    PaymentService paymentService;
    OrderRepository orderRepository;

    void SetUp() override {
        filmList = FilmList();
        sessionList = SessionList();
        seatList = SeatList();
        authService = UserAuthService();
        paymentService = PaymentService();
        orderRepository = OrderRepository();

        booking = CinemaBooking(filmList, sessionList, seatList,
                authService, paymentService, orderRepository);
        booking.newBooking();
    }
};

// Column 0-2: Film does not exist -> Error "Film not found"
TEST_F(CinemaBookingTest, Column0_2_FilmNotFound) {
    string invalidFilm = "Non-existent film";

    bool isFilmValid = booking.isFilmExists(invalidFilm);

    EXPECT_FALSE(isFilmValid) << "Film should not exist";
}

// Column 3: Session ended -> Error "Session already ended"
TEST_F(CinemaBookingTest, Column3_SessionEnded) {
    string validFilm = "Avatar";
    string expiredSession = "09:00";

    booking.setFilm(validFilm);
    bool filmExists = booking.isFilmExists(validFilm);
    bool sessionActive = booking.isSessionActive(expiredSession);

    EXPECT_TRUE(filmExists) << "Film should exist";
    EXPECT_FALSE(sessionActive) << "Session should not be active";
}

// Column 4: Seats occupied -> Error "Seats unavailable"
TEST_F(CinemaBookingTest, Column4_SeatsUnavailable) {
    string validFilm = "Avatar";
    string activeSession = "18:00";
    vector<int> bookedSeats = {1, 2, 3};

    booking.setFilm(validFilm);
    booking.setSession(activeSession);
    booking.selectSeats(bookedSeats);

    bool filmExists = booking.isFilmExists(validFilm);
    bool sessionActive = booking.isSessionActive(activeSession);
    bool seatsAvailable = booking.areSeatsAvailable(bookedSeats);

    EXPECT_TRUE(filmExists) << "Film should exist";
    EXPECT_TRUE(sessionActive) << "Session should be active";
    EXPECT_FALSE(seatsAvailable) << "Seats should not be available";
}

// Column 5: User not authorized -> Error "User not authorized"
TEST_F(CinemaBookingTest, Column5_UserNotAuthorized) {
    string validFilm = "Avatar";
    string activeSession = "18:00";
    vector<int> freeSeats = {10, 11, 12};

    booking.setFilm(validFilm);
    booking.setSession(activeSession);
    booking.selectSeats(freeSeats);

    bool filmExists = booking.isFilmExists(validFilm);
    bool sessionActive = booking.isSessionActive(activeSession);
    bool seatsAvailable = booking.areSeatsAvailable(freeSeats);
    bool userAuthorized = booking.isUserAuthorized();

    EXPECT_TRUE(filmExists) << "Film should exist";
    EXPECT_TRUE(sessionActive) << "Session should be active";
    EXPECT_TRUE(seatsAvailable) << "Seats should be available";
    EXPECT_FALSE(userAuthorized) << "User should not be authorized";
}

// Column 6: Insufficient funds -> Error "Insufficient funds"
TEST_F(CinemaBookingTest, Column6_InsufficientFunds) {
    string validFilm = "Avatar";
    string activeSession = "18:00";
    vector<int> freeSeats = {10, 11, 12};
    string authorizedUser = "user@example.com";
    double insufficientFunds = 50.0;

    authService.authorizeUser(authorizedUser);
    booking.setFilm(validFilm);
    booking.setSession(activeSession);
    booking.selectSeats(freeSeats);
    booking.setUser(authorizedUser);

    bool filmExists = booking.isFilmExists(validFilm);
    bool sessionActive = booking.isSessionActive(activeSession);
    bool seatsAvailable = booking.areSeatsAvailable(freeSeats);
    bool userAuthorized = booking.isUserAuthorized();
    bool hasFunds = booking.hasSufficientFunds(insufficientFunds);

    EXPECT_TRUE(filmExists) << "Film should exist";
    EXPECT_TRUE(sessionActive) << "Session should be active";
    EXPECT_TRUE(seatsAvailable) << "Seats should be available";
    EXPECT_TRUE(userAuthorized) << "User should be authorized";
    EXPECT_FALSE(hasFunds) << "Funds should be insufficient";
}

// Column 7: All conditions met -> Successful booking
TEST_F(CinemaBookingTest, Column7_SuccessfulBooking) {
    string validFilm = "Avatar";
    string activeSession = "18:00";
    vector<int> freeSeats = {10, 11, 12};
    string authorizedUser = "user@example.com";
    double sufficientFunds = 300.0;

    authService.authorizeUser(authorizedUser);
    booking.setFilm(validFilm);
    booking.setSession(activeSession);
    booking.selectSeats(freeSeats);
    booking.setUser(authorizedUser);

    bool filmExists = booking.isFilmExists(validFilm);
    bool sessionActive = booking.isSessionActive(activeSession);
    bool seatsAvailable = booking.areSeatsAvailable(freeSeats);
    bool userAuthorized = booking.isUserAuthorized();
    bool hasFunds = booking.hasSufficientFunds(sufficientFunds);

    EXPECT_TRUE(filmExists) << "Film should exist";
    EXPECT_TRUE(sessionActive) << "Session should be active";
    EXPECT_TRUE(seatsAvailable) << "Seats should be available";
    EXPECT_TRUE(userAuthorized) << "User should be authorized";
    EXPECT_TRUE(hasFunds) << "Funds should be sufficient";

    Order order = booking.createBooking();
    EXPECT_FALSE(order.isEmpty()) << "Order should be created";
    EXPECT_EQ(order.getFilm(), validFilm) << "Film in order should match";
    EXPECT_EQ(order.getSession(), activeSession) << "Session in order should match";
}

// Test validation of all available films
TEST_F(CinemaBookingTest, AllFilmsAreValid) {
    vector<string> expectedFilms = {
        "Avatar",
        "Interstellar",
        "The Matrix",
        "Titanic"
    };

    for (const auto& film : expectedFilms) {
        EXPECT_TRUE(booking.isFilmExists(film))
            << "Film '" << film << "' should be valid";
    }
}

// Test validation of all available sessions
TEST_F(CinemaBookingTest, AllSessionsAreValid) {
    vector<string> expectedSessions = {
        "10:00",
        "13:00",
        "16:00",
        "19:00"
    };

    string validFilm = "Avatar";
    booking.setFilm(validFilm);

    for (const auto& session : expectedSessions) {
        EXPECT_TRUE(booking.isSessionExists(session))
            << "Session '" << session << "' should exist";
    }
}

// Test ticket price calculation
TEST_F(CinemaBookingTest, TicketPriceCalculation) {
    string validFilm = "Avatar";
    vector<int> seats = {1, 2, 3};

    booking.setFilm(validFilm);
    booking.selectSeats(seats);

    double expectedPrice = 300.0;
    double actualPrice = booking.calculateTotalPrice();

    EXPECT_DOUBLE_EQ(actualPrice, expectedPrice)
        << "Price should be calculated correctly";
}

// Test error when invalid seats are selected
TEST_F(CinemaBookingTest, InvalidSeatsReturnsError) {
    string validFilm = "Avatar";
    vector<int> invalidSeats = {-1, 100};

    booking.setFilm(validFilm);
    
    EXPECT_FALSE(booking.selectSeats(invalidSeats))
        << "Invalid seats should return error";
}

// Test error when invalid film is selected
TEST_F(CinemaBookingTest, InvalidFilmReturnsError) {
    string invalidFilm = "Non-existent film";
    
    EXPECT_FALSE(booking.setFilm(invalidFilm))
        << "Invalid film should return error";
}

// Test booking operations are logged
TEST_F(CinemaBookingTest, BookingOperationsLogged) {
    string validFilm = "Avatar";
    booking.setFilm(validFilm);
    
    string logs = booking.getLogs();
    
    EXPECT_THAT(logs, ::testing::HasSubstr("Film selected"))
        << "Film selection operation should be logged";
}
